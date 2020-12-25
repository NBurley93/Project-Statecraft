import os
import sys
import struct
import glob
import zlib

class BankTableEntry():
    def __init__(self):
        self.idSize = 0
        self.id = ''
        self.location = 0
        self.dataSize = 0
        self.data = b''


audioBankData = b''

audioBankData += 'SND'.encode()
audioBankData += struct.pack('<B', 0x01)

numSounds = 0
dataHead = 12
rawData = b''

bankName = ''
bankPath = ''
bankFilePaths = []
bankTable = []

tableOffset = 0

if len(sys.argv) == 3:
    bankPath = sys.argv[1]
    bankName = sys.argv[2]

    for f in glob.glob(os.path.join(bankPath, '*.wav'), recursive=True):
        bankFilePaths.append(f)

    print('Creating sound-bank: {}...'.format(bankName))
    for fi in bankFilePaths:
        print('Processing {}'.format(os.path.basename(fi)))
        dat = zlib.compress(open(fi, 'rb').read(), 9)
        te = BankTableEntry()

        te.id = os.path.basename(fi)
        te.idSize = len(te.id)
        te.location = tableOffset
        te.dataSize = len(dat)
        te.data = dat
        tableOffset += te.dataSize
        entrySize = (4 * 3) + te.idSize
        dataHead += entrySize


        bankTable.append(te)
    numSounds = len(bankTable)

    audioBankData += struct.pack('<I', numSounds)
    audioBankData += struct.pack('<I', dataHead)

    # Write table
    for tb in bankTable:
        audioBankData += struct.pack('<I', tb.idSize)
        audioBankData += tb.id.encode()
        audioBankData += struct.pack('<I', tb.location)
        audioBankData += struct.pack('<I', tb.dataSize)
    
    # Write data
    for d in bankTable:
        audioBankData += tb.data


    with open(bankName + '.soundbank', 'wb') as fi:
        fi.write(audioBankData)
    print('Created soundbank {}.soundbank'.format(bankName))
    print('Statistics:\n\tFilesize: {0} KB\n\tNum Sounds: {1}\n\tData-head: {2}'.format(
        len(audioBankData) * 0.001,
        numSounds,
        dataHead
    ))
else:
    print('usage: soundcrunch <path-to-sounds> <bank-name>')