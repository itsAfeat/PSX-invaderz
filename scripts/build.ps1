$xml_file = 0

Get-ChildItem .\build\ -Filter *.xml -Recurse -File -Name| ForEach-Object {
    $xml_file = [System.IO.Path]::GetFileName($_)
}

cmake --preset default .
cmake --build .\build
mkpsxiso -o game.iso -lba LBA.log -y .\build\$xml_file
New-Item -ItemType Directory -Force -Path .\release| Out-Null
Move-Item -Path "game.iso" -Destination .\release\game.iso -force