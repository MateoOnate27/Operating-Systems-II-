import os
# System Call: open
fd = os.open("sample.txt", os.O_RDONLY)
# System Call: read
data = os.read(fd, 128)
# System Call: write (1 = stdout)
os.write(1, data)
# System Call: close
os.close(fd)
