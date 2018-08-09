Binary file format
==================

| Offset | Size(bytes) | Purpose |
| ------ |:-----------:| ----------------:|
|  0x0               |      4                        | Size of data section |
|  0x4               |      4                        | Size of text section |
|  0x8               | Size of data section in bytes | The data section     |
|  0xC + SIZE_OF_DS  | Size of text section in bytes | Text section         |
