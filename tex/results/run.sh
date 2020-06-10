#!/bin/bash
echo "Generating PDF..."
pdflatex -jobname=particle-mesh-results particle-mesh-results.tex
bibtex particle-mesh-results
pdflatex -jobname=particle-mesh-results particle-mesh-results.tex
pdflatex -jobname=particle-mesh-results particle-mesh-results.tex
