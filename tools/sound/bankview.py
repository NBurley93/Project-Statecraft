import struct
import os
import sys

SUPPORTED_VERSION = 0x01

if not len(sys.argv) == 2:
    print('usage: bankview <sound-bank>')
else:
    fh = open(sys.argv[1], 'rb')
    headid = fh.read(3)
    if headid == b'SND':
        # Valid bank
        fVersion = struct.unpack('<B', fh.read(1))[0]
        if fVersion <= SUPPORTED_VERSION:
            # Supported version
            numSounds = struct.unpack('<I', fh.read(4))[0]
            dataHead = struct.unpack('<I', fh.read(4))[0]

            print('Sounds in bank: {}'.format(numSounds))
            sndTbl = []

            for i in range(numSounds):
                idSize = struct.unpack('<I', fh.read(4))[0]
                tid = fh.read(idSize).decode()
                loc = struct.unpack('<I', fh.read(4))[0]
                csize = struct.unpack('<I', fh.read(4))[0]

                sndTbl.append({
                    'id': tid,
                    'location': loc,
                    'data-size': csize
                })

            print(sndTbl)
        else:
            print('Unsupported version!')
    else:
        print('Invalid or corrupt soundbank detected!')
    fh.close()