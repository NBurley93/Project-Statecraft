# TODO: Convert this into a proper native program, along with all asset-creation tools
import struct
import zlib
import sys
import os
from tqdm import tqdm
from PIL import Image

from queue import Queue, Empty
import threading
from threading import Thread


class Worker(Thread):
    _TIMEOUT = 2
    """ Thread executing tasks from a given tasks queue. Thread is signalable, 
        to exit
    """
    def __init__(self, tasks, th_num):
        Thread.__init__(self)
        self.tasks = tasks
        self.daemon, self.th_num = True, th_num
        self.done = threading.Event()
        self.start()

    def run(self):       
        while not self.done.is_set():
            try:
                func, args, kwargs = self.tasks.get(block=True,
                                                   timeout=self._TIMEOUT)
                try:
                    func(*args, **kwargs)
                except Exception as e:
                    print(e)
                finally:
                    self.tasks.task_done()
            except Empty as e:
                pass
        return

    def signal_exit(self):
        """ Signal to thread to exit """
        self.done.set()


class ThreadPool:
    """Pool of threads consuming tasks from a queue"""
    def __init__(self, num_threads, tasks=[]):
        self.tasks = Queue(num_threads)
        self.workers = []
        self.done = False
        self._init_workers(num_threads)
        for task in tasks:
            self.tasks.put(task)

    def _init_workers(self, num_threads):
        for i in range(num_threads):
            self.workers.append(Worker(self.tasks, i))

    def add_task(self, func, *args, **kwargs):
        """Add a task to the queue"""
        self.tasks.put((func, args, kwargs))

    def _close_all_threads(self):
        """ Signal all threads to exit and lose the references to them """
        for workr in self.workers:
            workr.signal_exit()
        self.workers = []

    def wait_completion(self):
        """Wait for completion of all the tasks in the queue"""
        self.tasks.join()

    def __del__(self):
        self._close_all_threads()


def create_task(func, *args, **kwargs):
    return (func, args, kwargs)


processedChunks = {}
COCURRENT_WORKERS = 50
MAX_CHUNK_SIZE = 4096


def chunks(lst, n):
    return [lst[i:i + n] for i in range(0, len(lst), n)]


def process_chunk(chunk, loc):
    global processedChunks
    ps = b''
    for px in chunk:
        pxl = (px[0] << 24) | (px[1] << 16) | (px[2] << 8) | px[3]
        ps += struct.pack('<L', pxl)
    processedChunks[loc] = ps


if len(sys.argv) != 2:
    print('usage: bitmap.py <source-file>')
else:
    fpath = sys.argv[1]
    if os.path.exists(fpath):
        print('Opening source image')
        im = Image.open(fpath)
        if not im.mode == 'RGBA':
            print('Converting to explicit-alpha channeled image, was previously {}'.format(im.mode))
            im = im.convert(mode='RGBA')
        
        nPixels = im.width * im.height
        rawPixelData = im.tobytes()

        pixels = b''
        dta = list(im.getdata())

        pixelChunks = list(chunks(dta, MAX_CHUNK_SIZE))

        jobPool = ThreadPool(COCURRENT_WORKERS)
        for c in pixelChunks:
            jobPool.add_task(process_chunk, c, pixelChunks.index(c))
        jobPool.wait_completion()
        for k in sorted(processedChunks):
            pixels += processedChunks[k]

        print('Bitmap-data: {} bytes of pixel data'.format(len(pixels)))
        fname = os.path.splitext(os.path.basename(fpath))[0]

        print('Compressing pixel-data')
        cpxlcrc = zlib.crc32(pixels)
        cpixels = zlib.compress(pixels)
        print('Compressed-pixel-size: {}'.format(len(cpixels)))

        print('Creating bitmap file'.format(fname))
        with open(os.path.join('game/textures/', fname) + '.bitmap', 'wb') as fi:
            fi.write(b'BITMAP')
            fi.write(struct.pack('<I', 1))
            fi.write(struct.pack('<I', im.width))
            fi.write(struct.pack('<I', im.height))
            fi.write(struct.pack('<I', cpxlcrc))
            fi.write(cpixels)

        print('Done')
    else:
        print('Provided file does not exist')