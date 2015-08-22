Binary file format
==================

| Offset | Size(bytes) | Purpose |
| ------ |:-----------:| ----------------:|
|  0x0               |      4                        | Size of data section |
|  0x5               |      4                        | Size of text section |
|  0x9               | Size of data section in bytes | The data section     |
|  0xD + SIZE_OF_DS  | Size of text section in bytes | Text section         |