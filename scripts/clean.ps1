$prg_name=$args[0]

Remove-Item .\release\game.iso
Remove-Item .\build\$prg_name.*

Remove-Item *.log
Remove-Item *.mcd
Remove-Item offscreen.*
Remove-Item output.*
Remove-Item vram-viewer.*

Write-Host "Project has been cleaned..."