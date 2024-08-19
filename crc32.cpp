//creditos: https://github.com/bakercp/CRC32/ 

// Condicionalmente usar memória de programa se estiver disponível.
#if defined(PROGMEM)
    #define FLASH_PROGMEM PROGMEM
    #define FLASH_READ_DWORD(x) (pgm_read_dword_near(x))
#else
    #define FLASH_PROGMEM
    #define FLASH_READ_DWORD(x) (*(uint32_t*)(x))
#endif

// Tabela de CRC32 armazenada em memória de programa (se disponível).
static const uint32_t crc32_table[] FLASH_PROGMEM = {
    0x00000000, 0x1db71064, 0x3b6e20c8, 0x26d930ac,
    0x76dc4190, 0x6b6b51f4, 0x4db26158, 0x5005713c,
    0xedb88320, 0xf00f9344, 0xd6d6a3e8, 0xcb61b38c,
    0x9b64c2b0, 0x86d3d2d4, 0xa00ae278, 0xbdbdf21c
};

/**
 * @brief Classe para calcular o checksum CRC32 a partir de dados arbitrários.
 * @sa http://forum.arduino.cc/index.php?topic=91179.0
 */
class CRC32
{
public:
    /**
     * @brief Construtor da classe.
     * Inicializa um checksum CRC32 vazio.
     */
    CRC32() 
    {
        reset();
    }

    /**
     * @brief Reinicia o cálculo do checksum.
     */
    void reset() 
    {
        state_ = ~0L;
    }

    /**
     * @brief Atualiza o cálculo do checksum atual com os dados fornecidos.
     * @param data Os dados a serem adicionados ao checksum.
     */
    void update(const uint8_t& data) 
    {
        // via http://forum.arduino.cc/index.php?topic=91179.0
        uint8_t tbl_idx = state_ ^ (data >> (0 * 4));
        state_ = FLASH_READ_DWORD(crc32_table + (tbl_idx & 0x0f)) ^ (state_ >> 4);
        tbl_idx = state_ ^ (data >> (1 * 4));
        state_ = FLASH_READ_DWORD(crc32_table + (tbl_idx & 0x0f)) ^ (state_ >> 4);
    }

    /**
     * @brief Atualiza o cálculo do checksum atual com os dados fornecidos.
     * @tparam Type O tipo de dado a ser lido.
     * @param data Os dados a serem adicionados ao checksum.
     */
    template <typename Type>
    void update(const Type& data)
    {
        update(&data, 1);
    }

    /**
     * @brief Atualiza o cálculo do checksum atual com os dados fornecidos.
     * @tparam Type O tipo de dado a ser lido.
     * @param data O array de dados a ser adicionado ao checksum.
     * @param size O tamanho do array a ser adicionado.
     */
    template <typename Type>
    void update(const Type* data, size_t size)
    {
        size_t n_bytes = size * sizeof(Type);
        const uint8_t* p_data = reinterpret_cast<const uint8_t*>(data);

        for (size_t i = 0; i < n_bytes; i++)
        {
            update(p_data[i]);
        }
    }

    /**
     * @brief Finaliza o cálculo e retorna o checksum calculado.
     * @return O checksum calculado.
     */
    uint32_t finalize() const 
    {
        return ~state_;
    }

    /**
     * @brief Calcula o checksum de um array de dados arbitrários.
     * @tparam Type O tipo de dado a ser lido.
     * @param data Um ponteiro para os dados a serem adicionados ao checksum.
     * @param size O tamanho dos dados a serem adicionados ao checksum.
     * @return O checksum calculado.
     */
    template <typename Type>
    static uint32_t calculate(const Type* data, size_t size)
    {
        CRC32 crc;
        crc.update(data, size);
        return crc.finalize();
    }

private:
    /**
     * @brief Estado interno do checksum.
     */
    uint32_t state_ = ~0L;
};
