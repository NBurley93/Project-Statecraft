#include <pch.hh>
#include "AudioBank.hh"




AudioBank* LoadAudioBank(const std::string& filepath) {
    std::ifstream fs(filepath, std::ios::binary);
    if (!fs.is_open()) {
        GAME_ERROR("Could not open AudioBank: {}", filepath);
    }

    std::vector<unsigned char> bankBuffer((std::istreambuf_iterator<char>(fs)),
                                           std::istreambuf_iterator<char>());
    fs.close();

    AudioBank* bnk = new AudioBank();
    bnk->fileid[0] = bankBuffer.at(0);
    bnk->fileid[1] = bankBuffer.at(1);
    bnk->fileid[2] = bankBuffer.at(2);
    bnk->version = bankBuffer.at(3);
    memcpy(&bnk->numSounds, bankBuffer.data()+4, 4);
    memcpy(&bnk->dataHead, bankBuffer.data()+8, 4);
    bnk->bankTable = std::vector<BankTableEntry*>();
    bnk->rawData = std::vector<unsigned char>();

    int offset = 12;
    uint32_t totalDataSize = 0;
    for (auto i = 0; i < bnk->numSounds; ++i) {
        BankTableEntry *et = new BankTableEntry();
        memcpy(&et->idSize, bankBuffer.data()+offset, 4);
        et->id = new char[et->idSize + 1];
        strncpy(et->id, (const char*)bankBuffer.data()+offset+4, et->idSize);
        offset += 4 + et->idSize;
        et->id[et->idSize] = '\0';
        memcpy(&et->location, bankBuffer.data()+offset, 4);
        memcpy(&et->dataSize, bankBuffer.data()+offset+4, 4);
        totalDataSize += et->dataSize;
        offset += (4 * 2);
        bnk->bankTable.push_back(et);
    }
    bnk->rawData = std::vector<unsigned char>(bankBuffer.begin()+bnk->dataHead, bankBuffer.end());

    return bnk;
}
