main.pdf: body.pdf results.pdf ref.bib main.tex
	pdflatex main.tex
	bibtex main.aux
	pdflatex main.tex
	pdflatex main.tex

body.pdf: body.tex
	pdflatex body.tex

results.pdf: table.pdf graph1.pdf graph2.pdf results.tex
	pdflatex results.tex

table.pdf: table.tex
	pdflatex table.tex

graph1.pdf: plots/data1.plt
	gnuplot plots/data1.plt


graph2.pdf: plots/data2.plt
	gnuplot plots/data2.plt

.PHONY: view clean
view:
	xdg-open main.pdf

clean:
	rm graph1.pdf graph2.pdf
	rm table.pdf body.pdf results.pdf
	rm *.aux *.log *.bbl *.blg
