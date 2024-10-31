import serial
import time
from datetime import datetime

# Substitua 'COM3' pelo nome correto da porta serial, se necessário
porta_serial = 'COM3'
baud_rate = 9600

try:
    ser = serial.Serial(porta_serial, baud_rate)
    print(f"Conectado à porta serial: {porta_serial}")
except serial.SerialException as e:
    print(f"Erro ao conectar na porta {porta_serial}: {e}")
    exit()

# Aguarda um momento para a conexão estabilizar
time.sleep(2)

while True:
    if ser.in_waiting > 0:
        mensagem = ser.readline().decode('utf-8').strip()  # Lê a mensagem do Arduino
        hora_atual = datetime.now().strftime("%Y-%m-%d %H:%M:%S")  # Captura a hora do sistema
        print(f"{hora_atual} - {mensagem}")  # Imprime a hora e a mensagem

        # Registra no arquivo
        with open("registros.txt", "a") as arquivo:
            arquivo.write(f"{hora_atual} - {mensagem}\n")
