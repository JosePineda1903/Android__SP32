#include <BluetoothSerial.h>

BluetoothSerial SerialBT;

const int sensorPin = 4; // Pin D2 (GPIO 4) del ESP32
const int relayPin = 14; // Pin D14 (GPIO 14) del ESP32
unsigned long previousMillis = 0; // Variable para almacenar el tiempo del último procesamiento de comando
const long interval = 1000; // Intervalo de 1 segundo
bool motorEncendido = false; // Variable para rastrear el estado del motor
bool automaticMode = false; // Variable para rastrear si el motor está en modo automático o manual

void setup() {
    Serial.begin(9600);
    SerialBT.begin("ESP32 Humedad");

    pinMode(relayPin, OUTPUT); // Configurar el pin del relé como salida
}

void loop() {
    unsigned long currentMillis = millis(); // Obtener el tiempo actual
    if (currentMillis - previousMillis >= interval) {
        // Si ha pasado al menos 1 segundo desde el último procesamiento de comando
        previousMillis = currentMillis; // Actualizar el tiempo del último procesamiento

        // Procesar comandos Bluetooth
        if (SerialBT.available()) {
            char command = SerialBT.read(); // Leer el comando recibido
            if (command == '1') {
                // Cambiar a modo manual y encender el motor
                automaticMode = false;
                encenderMotor();
                SerialBT.println("Relé encendido manualmente");
            } else if (command == '0') {
                // Cambiar a modo manual y apagar el motor
                automaticMode = false;
                apagarMotor();
                SerialBT.println("Relé apagado manualmente");
            } else if (command == 'A') {
                // Activar modo automático
                automaticMode = true;
                SerialBT.println("Modo automático activado");
            } else if (command == 'M') {
                // Desactivar modo automático
                automaticMode = false;
                SerialBT.println("Modo automático desactivado");
            }
        }

        // Control automático basado en la humedad
        if (automaticMode) {
            float humidity = obtenerHumedad();
            if (humidity > 20 && !motorEncendido) {
                encenderMotor();
                SerialBT.println("Relé encendido automáticamente debido a la humedad baja");
            } else if (humidity <= 20 && motorEncendido) {
                apagarMotor();
                SerialBT.println("Relé apagado automáticamente debido a la humedad adecuada");
            }
        }

        // Obtener la humedad y enviarla por Bluetooth
        float humidity = obtenerHumedad();
        SerialBT.print("Humedad: ");
        SerialBT.print(humidity);
        SerialBT.println("%");

        // Mostrar la humedad en el monitor serie
        Serial.print("Humedad: ");
        Serial.print(humidity);
        Serial.println("%");
    }
}

float obtenerHumedad() {
    int sensorValue = analogRead(sensorPin);
    // Ajustamos el mapeo de manera inversa para obtener la humedad en un rango de 0 a 100
    return map(sensorValue, 0, 4095, 100, 0);
}

void encenderMotor() {
    digitalWrite(relayPin, HIGH); // Encender el motor
    motorEncendido = true; // Actualizar el estado del motor
}

void apagarMotor() {
    digitalWrite(relayPin, LOW); // Apagar el motor
    motorEncendido = false; // Actualizar el estado del motor
}
