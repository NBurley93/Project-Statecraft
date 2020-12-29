#pragma once


struct BankTableEntry {
    uint32_t idSize;
    char* id;
    uint32_t location;
    uint32_t dataSize;
};


struct AudioBank {
    char fileid[3];
    uint8_t version;
    uint32_t numSounds;
    uint32_t dataHead;
    std::vector<BankTableEntry*> bankTable;
    std::vector<unsigned char> rawData;
};


AudioBank* LoadAudioBank(const std::string& filepath);