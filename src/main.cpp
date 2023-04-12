#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>

// Define os pinos do módulo NRF24L01
#define CE_PIN 9
#define CSN_PIN 10

// Cria uma instância do objeto RF24L01
RF24 RF24Radio(CE_PIN, CSN_PIN);

// Endereço de comunicação entre as placas (Max 5 bytes)
const byte address[6] = "00001";

// Determina se a placa irá receber ou enviar dados
const int RF24Type = 0;

void RF24ReadMensage();
void RF24SendMensage(String mensage);

void setup() {

    Serial.begin(9600);

    // Inicializa o módulo NRF24L01
    RF24Radio.begin();

    // Define a potência do módulo: 
    // RF24_PA_MIN (-18 dBm), RF24_PA_LOW (-12 dBm), RF24_PA_HIGH (-6 dBm) e RF24_PA_MAX (0 dBm)
    RF24Radio.setPALevel(RF24_PA_MIN);

    // Define a taxa de transmissão do módulo: 
    // RF24_250KBPS, RF24_1MBPS e RF24_2MBPS
    // RF24Radio.setDataRate(RF24_250KBPS);

    // Define o tamanho máximo do payload (dados a serem transmitidos) padrão é 32 bytes
    // RF24Radio.setPayloadSize(16);

    // Ativa ou desativa a funcionalidade de auto-acknowledgment (auto-Ack) 
    // para confirmação automática de recebimento de pacotes.
    // RF24Radio.setAutoAck(false);

    //  Define o tempo de espera entre retransmissões de pacotes e o número máximo de retransmissões permitidas.
    // RF24Radio.setRetries(100, 10);

    // Define o tamanho do campo de verificação de redundância cíclica (CRC) 
    // para verificar a integridade dos dados transmitidos. As opções possíveis são: 
    // RF24_CRC_DISABLED (desabilitado), RF24_CRC_8BITS (8 bits) e RF24_CRC_16BITS (16 bits).
    // RF24Radio.setCRCLength(RF24_CRC_DISABLED);

    if(RF24Type == 0) {
        // Define o endereço de escrita (receptor)
        RF24Radio.openWritingPipe(address);
        // Entra no modo de transmissão
        RF24Radio.stopListening();
    } else {
        // Define o endereço de leitura (emissor)
        RF24Radio.openReadingPipe(0, address);
        // Inicia o modo de escuta
        RF24Radio.startListening();
    }

}

void loop() {
    if(RF24Type == 0) {
        RF24SendMensage("Olá Mundo!");
        delay(500);
    } else {
        RF24ReadMensage();
    }
}

void RF24SendMensage(String mensage) {
    // Envia os dados informados
    if (RF24Radio.write(mensage.c_str(), mensage.length() + 1)) {
        Serial.println("Dados enviados com sucesso!");
    } else {
        Serial.println("Falha ao enviar os dados.");
    }
}

void RF24ReadMensage() {
    // Verifica se há dados disponíveis para leitura
    if (RF24Radio.available()) {
        // Cria um buffer para armazenar os dados recebidos
        char buffer[32];
        // Lê os dados recebidos
        RF24Radio.read(&buffer, sizeof(buffer));
        // Escreve os dados recebidos na serial
        Serial.println("Dados recebidos: " + String(buffer));
    }
}