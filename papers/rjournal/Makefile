all: clean RJwrapper.pdf

clean:
	rm -fr RJwrapper.pdf
	rm -fr RJwrapper.out
	rm -fr RJwrapper.aux
	rm -fr RJwrapper.log

RJwrapper.pdf: RJwrapper.tex FrancoisEddelbuettel.tex RJournal.sty
	pdflatex RJwrapper.tex
	bibtex RJwrapper
	pdflatex RJwrapper.tex
	pdflatex RJwrapper.tex

