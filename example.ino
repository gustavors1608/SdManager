#include "sdManager.cpp"  

SdManager sd_manager(10);  // Pino 10 como exemplo para o CS

void setup() {
    Serial.begin(9600);

    // Inicializa o SD Card
    if (!sd_manager.begin()) {
        Serial.println("Falha ao inicializar o cartão SD!");
        return;
    }
    Serial.println("Cartão SD inicializado com sucesso.");

    // 1. Criar e escrever dados em um arquivo CSV
    if (sd_manager.append_to_file("data.csv", "coluna1,coluna2,coluna3")) {
        Serial.println("Dados iniciais escritos em data.csv.");
    }

    // 2. Adicionar mais dados ao arquivo CSV
    if (sd_manager.append_to_file("data.csv", "10,20,30")) {
        Serial.println("Dados adicionados a data.csv.");
    }

    // 3. Calcular o CRC32 do arquivo CSV
    uint32_t crc = sd_manager.calculate_crc32("data.csv");
    Serial.print("CRC32 de data.csv: ");
    Serial.println(crc, HEX);

    // 4. Criar um arquivo de configuração .ini e definir valores
    if (sd_manager.set_config_value("config.ini", "username", "admin")) {
        Serial.println("Configuração 'username' definida em config.ini.");
    }
    if (sd_manager.set_config_value("config.ini", "password", "12345")) {
        Serial.println("Configuração 'password' definida em config.ini.");
    }

    // 5. Obter valores de configuração
    String username = sd_manager.get_config_value("config.ini", "username");
    Serial.print("Valor de 'username' em config.ini: ");
    Serial.println(username);

    String password = sd_manager.get_config_value("config.ini", "password");
    Serial.print("Valor de 'password' em config.ini: ");
    Serial.println(password);

    // 6. Buscar e substituir valores no arquivo de configuração
    if (sd_manager.search_and_replace("config.ini", "password", "senha")) {
        Serial.println("Substituição do identificador de 'password' em config.ini realizada.");
    }

    // 7. Criar um backup do arquivo de configuração (realizar antes de fazer uma alteração critica, em um arquivo que dita o funcionamento do sistema por exemplo, dai poderia voltar para o backup em caso de erro etc)
    if (sd_manager.create_backup("config.ini")) {
        Serial.println("Backup de config.ini criado com sucesso.");
    }
}

void loop() {
    // Nenhuma operação no loop para este exemplo
}
