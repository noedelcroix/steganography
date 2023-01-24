# sysg5 - g55990 - Delcroix - Stéganographie
A project made for my system course. There are two kind of steganography in this project : LSB on PNG files and homemade white-space characters steganography.
## Génération PDF
```bash
cd rapport
pdflatex -shell-escape -interaction=nonstopmode -file-line-error main.tex
bibtex main
pdflatex -shell-escape -interaction=nonstopmode -file-line-error main.tex
pdflatex -shell-escape -interaction=nonstopmode -file-line-error main.tex
```

## Démo et commandes
```bash
cd code

# Demo
make

# Build
make build

# Run
./steganography <zero−width|lsb> <decode|encode> <chemin fichier> <texte optionnel>
```

## Bibliographie
### Idées de stéganographie :
- [root-me](https://www.root-me.org/fr/Challenges/Steganographie/)
### PNG format :
- [w3c](https://www.w3.org/TR/2003/REC-PNG-20031110/)
- [least significant bit](https://repository.root-me.org/St%C3%A9ganographie/EN%20-%20LSB%20Steganography.pdf)
- least significant bit - misc N°47 page 75
### JPEG format :
- [corkami/formats](https://github.com/corkami/formats/blob/master/image/jpeg.md)
### Text :
- [whitespace](https://manpages.ubuntu.com/manpages/bionic/man1/stegsnow.1.html)

## Idées de projet
- Stéganographie LSB sur PNG, decoder et encoder.
- JPEG ?
- Whitespace, decoder et encoder.
- ZIP ?
- MP3 et/ou MP4 ?
- Autres formats fonctionnant avec la compression Huffman.
