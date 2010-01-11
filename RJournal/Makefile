all: clean RJwrapper.pdf

clean:
	rm -fr RJwrapper.pdf
	rm -fr RJwrapper.out
	rm -fr RJwrapper.aux
	rm -fr RJwrapper.log

RJwrapper.pdf: RJwrapper.tex FrancoisEddelbuettel.tex RJournal.sty
	pdflatex RJwrapper.tex
	pdflatex RJwrapper.tex

