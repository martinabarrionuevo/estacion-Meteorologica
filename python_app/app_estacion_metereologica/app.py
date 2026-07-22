import serial
import mysql.connector


class EstacionReceptora:
    # Constructor: Inicializa las conexiones como atributos del objeto
    def __init__(self, puerto_com, baudios, db_host, db_user, db_pass, db_name):
        self.puerto_com = puerto_com
        self.baudios = baudios

        # Intentamos conectar con el hardware (Arduino)
        self.arduino = serial.Serial(puerto_com, baudios, timeout=1)

        # Intentamos conectar con la Base de Datos (MySQL)
        self.db = mysql.connector.connect(
            host=db_host,
            user=db_user,
            password=db_pass,
            database=db_name
        )

        self.cursor = self.db.cursor()

        print(f"📡 Objeto Estación creado. Conectado a {puerto_com} y MySQL con éxito.")

    # Método: Escucha el puerto USB y procesa el texto
    def recibir_y_guardar(self):
        # Leer línea desde el cable USB (vienen bytes, decodificamos a texto)
        linea = self.arduino.readline().decode('utf-8').strip()

        if linea:
            try:
                # El Arduino envía: humedad,temperatura (Ej: 55.00,24.00)
                datos = linea.split(',')

                humedad_act = float(datos[0])
                temperatura_act = float(datos[1])

                # Ejecutamos el método interno para insertar en MySQL
                self.__insertar_en_db(temperatura_act, humedad_act)

                print(
                    f"[NUEVA MEDICIÓN] Humedad: {humedad_act}% | "
                    f"Temp: {temperatura_act}°C ➔ ¡Guardado!"
                )

            except (ValueError, IndexError):
                # Ignora datos incompletos o ruidos eléctricos del inicio
                pass

    # Método privado: Se encarga exclusivamente del SQL
    def __insertar_en_db(self, temp, hum):
        sql = "INSERT INTO mediciones (temperatura, humedad) VALUES (%s, %s)"
        self.cursor.execute(sql, (temp, hum))
        self.db.commit()  # Confirmamos el guardado


# --- EJECUCIÓN DEL PROGRAMA PRINCIPAL ---
if __name__ == "__main__":

    # Instanciamos el objeto 'mi_estacion'
    mi_estacion = EstacionReceptora(
        puerto_com='COM3',
        baudios=9600,
        db_host='127.0.0.1',
        db_user='root',
        db_pass='root',
        db_name='estacion_metereologica_proa'
    )

    print("🚀 Iniciando monitoreo continuo... Presioná Ctrl+C para detener.")

    # Bucle infinito de escucha utilizando el método del objeto
    while True:
        mi_estacion.recibir_y_guardar()