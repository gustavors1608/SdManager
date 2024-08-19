# SdManager 📂💾

**SdManager** é uma biblioteca Arduino fácil de usar que torna a manipulação de arquivos em cartões SD uma tarefa muito mais simples! 🎉 Com ela, você pode ler, escrever, calcular checksums CRC32, criar backups e outros! tudo de maneira eficiente. É a ferramenta perfeita para seus projetos IoT, garantindo que seus dados estejam seguros e intactos! 🔒✨

## Recursos ✨

- **📄 Leitura e Escrita de Arquivos**: Crie, leia, escreva e apague arquivos em cartões SD com simplicidade.
- **📝 Manipulação de Arquivos CSV**: Adicione dados ao final de arquivos CSV para criar logs ou armazenar dados continuamente.
- **⚙️ Configurações com Arquivos `.ini`**: Trabalhe com pares chave-valor em arquivos de configuração, com suporte para busca e substituição.
- **🔍 Cálculo de Checksums CRC32**: Verifique a integridade dos seus arquivos utilizando CRC32, garantindo que eles não foram corrompidos.
- **💾 Criação de Backups**: Crie cópias de segurança de arquivos importantes para evitar qualquer tipo de perda.

## Instalação 🚀

1. Baixe ou clone este repositório.
2. Copie a pasta `SdManager` para o diretório `libraries` do seu Arduino IDE.
3. No Arduino IDE, vá em **Sketch** -> **Incluir Biblioteca** -> **SdManager** para incluir a biblioteca no seu projeto.

## Exemplos de Uso 👨‍💻👩‍💻

### Inicialização e Manipulação Básica de Arquivos 📂

```cpp
#include "SdManager.h"

SdManager sd_manager(10);  // Pino 10 como exemplo para o CS

void setup() {
    Serial.begin(9600);

    if (!sd_manager.begin()) {
        Serial.println("Falha ao inicializar o cartão SD!");
        return;
    }
    Serial.println("Cartão SD inicializado com sucesso!");

    // Criação de um arquivo CSV e adição de dados
    if (sd_manager.append_to_file("data.csv", "coluna1,coluna2,coluna3")) {
        Serial.println("Dados iniciais escritos em data.csv.");
    }

    // Cálculo do CRC32 para verificar integridade do arquivo
    uint32_t crc = sd_manager.calculate_crc32("data.csv");
    Serial.print("CRC32 de data.csv: ");
    Serial.println(crc, HEX);
}

void loop() {
    // Código contínuo ou interações aqui
}
```

### Manipulação de Arquivos `.ini` para Configurações ⚙️

```cpp
void setup() {
    Serial.begin(9600);

    if (!sd_manager.begin()) {
        Serial.println("Falha ao inicializar o cartão SD!");
        return;
    }

    // Escrevendo e lendo configurações em arquivo .ini
    sd_manager.set_config_value("config.ini", "username", "admin");
    String username = sd_manager.get_config_value("config.ini", "username");
    Serial.print("Username: ");
    Serial.println(username);
}
```

### Backup de Arquivos 💾

```cpp
void setup() {
    Serial.begin(9600);

    if (!sd_manager.begin()) {
        Serial.println("Falha ao inicializar o cartão SD!");
        return;
    }

    // Criando um backup do arquivo de configuração
    if (sd_manager.create_backup("config.ini")) {
        Serial.println("Backup de config.ini criado com sucesso.");
    }
}
```

## API 📚

### Métodos Principais

- **`bool begin()`**: Inicializa a comunicação com o cartão SD.
- **`bool append_to_file(const char* file_name, const String& data)`**: Adiciona uma nova linha ao final de um arquivo.
- **`uint32_t calculate_crc32(const char* file_name)`**: Calcula o checksum CRC32 de um arquivo.
- **`bool set_config_value(const char* file_name, const String& key, const String& value)`**: Define o valor de uma chave específica em um arquivo `.ini`.
- **`String get_config_value(const char* file_name, const String& key)`**: Obtém o valor de uma chave específica em um arquivo `.ini`.
- **`bool create_backup(const char* file_name, const char* backup_suffix = "_backup")`**: Cria uma versão de backup de um arquivo.

## Contribuições 🤝

Contribuições são sempre bem-vindas! Sinta-se à vontade para enviar pull requests ou abrir issues no GitHub para sugestões e melhorias.

## Licença 📜

Este projeto está licenciado sob a licença MIT - veja o arquivo [LICENSE](LICENSE) para mais detalhes.

## Autor 👨‍💻

Criado por [Gustavo Stroschon](https://github.com/gustavors1608). 🚀
