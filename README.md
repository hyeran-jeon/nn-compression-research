# nn-compression-research
Researching optimal compression for neural network models.

    .
    ├── Huffman       # Files for huffman encoding overlay
    │   ├── bitstream       # .bit and .tcl files for overlay
    │   ├── hls             # HLS synthesis and co-simulation reports
    │   ├── ip              # IP files and AXI Lite memory map
    │   ├── notebook        # .ipynb Pynq notebook
    │   ├── src             # HLS source code
    │   └── vivado          # Vivado block diagram, summary, and memory map
    ├── LZW           # Files for lzw overlay
    │   ├── bitstream       # .bit and .tcl files for overlay
    │   ├── hls             # HLS synthesis and co-simulation reports
    │   ├── ip              # IP files and AXI Lite memory map
    │   ├── notebook        # .ipynb Pynq notebook
    │   ├── src             # HLS source code
    │   └── vivado          # Vivado block diagram, summary, and memory map
    ├── RLE           # Files for run length encoding overlay
    │   ├── bitstream       # .bit and .tcl files for overlay
    │   ├── hls             # HLS synthesis and co-simulation reports
    │   ├── ip              # IP files and AXI Lite memory map
    │   ├── notebook        # .ipynb Pynq notebook
    │   ├── src             # HLS source code
    │   └── vivado          # Vivado block diagram, summary, and memory map
    └── test          # Testing compression ratios with software compressors
        ├── programs        # Windows executables with a single input file parameter
        └── results         # Text results after running compression on various files  
