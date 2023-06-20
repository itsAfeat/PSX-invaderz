clear

cmake --preset default .
cmake --build ./build

xml_file=$(find -type f -wholename "./build/*.xml")
mkdir ./release
mkpsxiso -o invaderz.iso -lba LBA.log -y $xml_file
mv invaderz.iso ./release/invaderz.iso

~/PCSX-Redux.AppImage -run -stdout -iso ./release/invaderz.iso
rm *.mcd *.frag *.lua *.vert
