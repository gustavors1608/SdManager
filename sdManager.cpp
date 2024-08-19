#include <SPI.h>
#include <SD.h>
#include <Arduino.h>
#include <crc32.cpp> //https://github.com/bakercp/CRC32.git






/**
 * @brief Gerencia a manipulação de arquivos em um cartão SD.
 */
class SdManager {
public:
    /**
     * @brief Construtor da classe.
     * @param chip_select_pin Pino do chip select do módulo SD.
     */
    SdManager(byte chip_select_pin) {
        this->chip_select_pin = chip_select_pin;
    }

    /**
     * @brief Inicializa a comunicação com o cartão SD.
     * @return true se o cartão SD foi inicializado com sucesso, false caso contrário.
     */
    bool begin() {
        return SD.begin(chip_select_pin);
    }

    /**
     * @brief Calcula o CRC32 de um arquivo para verificação de integridade.
     * @param file_name Nome do arquivo cujo checksum será calculado.
     * @return Checksum CRC32 do arquivo, ou 0 se houver um erro na leitura.
     */
    uint32_t calculate_crc32(const char* file_name) {
        File file = open_file(file_name, FILE_READ);
        if (!file) return 0;

        CRC32 crc;
        while (file.available()) {
            crc.update(file.read());
        }
        file.close();
        return crc.finalize();
    }

    /**
     * @brief Busca e substitui uma palavra ou expressão em um arquivo.
     * @param file_name Nome do arquivo onde a substituição será feita.
     * @param search_str String a ser buscada.
     * @param replace_str String substituta.
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool search_and_replace(const char* file_name, const String& search_str, const String& replace_str) {
        String file_content = read_file(file_name);
        if (file_content.length() == 0) return false;

        file_content.replace(search_str, replace_str);
        return write_to_file(file_name, file_content, FILE_WRITE);
    }

    /**
     * @brief Cria uma versão de backup de um arquivo.
     * @param file_name Nome do arquivo original.
     * @param backup_suffix Sufixo para diferenciar a versão de backup (por exemplo, "_backup").
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool create_backup(const char* file_name, const char* backup_suffix = "_backup") {
        String backup_file_name = String(file_name) + String(backup_suffix);
        String file_content = read_file(file_name);
        if (file_content.length() == 0) return false;

        return write_to_file(backup_file_name.c_str(), file_content, FILE_WRITE);
    }


    /**
     * @brief Adiciona uma nova linha ao final de um arquivo.
     * @param file_name Nome do arquivo.
     * @param data Linha de dados a ser adicionada.
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool append_to_file(const char* file_name, const String& data) {
        return write_to_file(file_name, data + "\n", FILE_WRITE);
    }

    /**
     * @brief Obtém o valor de uma chave específica em um arquivo .ini.
     * @param file_name Nome do arquivo .ini.
     * @param key Chave cujo valor deve ser retornado.
     * @return Valor correspondente à chave, ou uma string vazia se a chave não for encontrada.
     */
    String get_config_value(const char* file_name, const String& key) {
        File file = open_file(file_name, FILE_READ);
        if (file) {
            String line;
            while (file.available()) {
                line = file.readStringUntil('\n');
                int separator_pos = line.indexOf('=');
                if (separator_pos != -1) {
                    String current_key = line.substring(0, separator_pos);
                    String value = line.substring(separator_pos + 1);
                    if (current_key == key) {
                        file.close();
                        return value;
                    }
                }
            }
            file.close();
        }
        return "";
    }

    /**
     * @brief Define o valor de uma chave específica em um arquivo .ini.
     * @param file_name Nome do arquivo .ini.
     * @param key Chave cujo valor deve ser definido.
     * @param value Novo valor a ser definido para a chave.
     * @return true se a operação foi bem-sucedida, false caso contrário.
     */
    bool set_config_value(const char* file_name, const String& key, const String& value) {
        String file_content = read_file(file_name);
        if (file_content.length() == 0) return false;

        String new_content = "";
        bool key_found = false;

        int current_pos = 0;
        int line_end_pos;
        while ((line_end_pos = file_content.indexOf('\n', current_pos)) != -1) {
            String line = file_content.substring(current_pos, line_end_pos);
            int separator_pos = line.indexOf('=');
            if (separator_pos != -1) {
                String current_key = line.substring(0, separator_pos);
                if (current_key == key) {
                    line = key + "=" + value;
                    key_found = true;
                }
            }
            new_content += line + "\n";
            current_pos = line_end_pos + 1;
        }

        if (!key_found) {
            new_content += key + "=" + value + "\n";
        }

        return write_to_file(file_name, new_content, FILE_WRITE);
    }

private:
    byte chip_select_pin;

    /**
     * @brief Abre um arquivo com verificação de erro.
     * @param file_name Nome do arquivo a ser aberto.
     * @param mode Modo de abertura do arquivo (leitura ou escrita).
     * @return Objeto File correspondente ao arquivo aberto, ou arquivo inválido se a abertura falhar.
     */
    File open_file(const char* file_name, const char* mode) {
        File file = SD.open(file_name, mode);
        if (!file) {
            Serial.println("Failed to open file: " + String(file_name));
        }
        return file;
    }

    /**
     * @brief Lê todo o conteúdo de um arquivo.
     * @param file_name Nome do arquivo a ser lido.
     * @return Conteúdo do arquivo em uma string, ou uma string vazia se a leitura falhar.
     */
    String read_file(const char* file_name) {
        File file = open_file(file_name, FILE_READ);
        String content = "";

        if (file) {
            while (file.available()) {
                content += file.readStringUntil('\n') + "\n";
            }
            file.close();
        }
        return content;
    }

    /**
     * @brief Escreve dados em um arquivo.
     * @param file_name Nome do arquivo a ser escrito.
     * @param data Dados a serem escritos no arquivo.
     * @param mode Modo de escrita (FILE_WRITE para adicionar ao final, ou outros modos).
     * @return true se a escrita foi bem-sucedida, false caso contrário.
     */
    bool write_to_file(const char* file_name, const String& data, const char* mode) {
        File file = open_file(file_name, mode);
        if (file) {
            file.print(data);
            file.close();
            return true;
        }
        return false;
    }
};
