# ch570_experiments

experiments on ch570 chip with the offical wch sdk

toolchain in makefile:
- https://github.com/ch32-riscv-ug/MounRiver_Studio_Community_miror/releases
    - v1.92, one patch too old
- https://github.com/cjacker/opensource-toolchain-ch32v
    - led to: http://file-oss.mounriver.com/tools/MRS_Toolchain_Linux_x64_V1.92.1.tar.xz


`make clean && make`

`python -m venv .venv`
install chprog in venv
`sudo .venv/bin/python chprog.py x.bin`



## Notes
From my tests the chip works all the way down to 1704mV, bluetooth only starts working around 1830mV.