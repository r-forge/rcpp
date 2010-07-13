
## There is an old bug in texidvi that makes it not swallow the ~
## marker used to denote whitespace. This is actually due to fixing
## another bug they whereby you could not run texidvi on directory
## names containing a tilde (as we happen to do for Debian builds of R
## alpha/beta/rc releases). The 'tilde' bug will go away as it
## reportedly has been squashed upstream but I am still bitten by it
## on Ubuntu so for Dirk will insist on pdflatex and this variable helps
whoami=$(shell whoami)

all: clean linkingto-proposal.pdf

clean:
	touch linkingto-proposal.pdf
	rm linkingto-proposal.pdf

linkingto-proposal.pdf : linkingto-proposal.Rnw
	Rscript -e "require(highlight); driver <- HighlightWeaveLatex(boxes = TRUE, bg = 'white' ); Sweave( 'linkingto-proposal.Rnw', driver = driver ); "
	Rscript -e "tools::texi2dvi( 'linkingto-proposal.tex', pdf = TRUE, clean = FALSE )"
	bibtex linkingto-proposal
ifeq ($(whoami),edd)
	pdflatex linkingto-proposal
	pdflatex linkingto-proposal
else
	Rscript -e "tools::texi2dvi( 'linkingto-proposal.tex', pdf = TRUE, clean = TRUE )"
endif
	rm -fr linkingto-proposal.tex
	rm -fr linkingto-proposal.bbl
	rm -fr linkingto-proposal.blg
	rm -fr linkingto-proposal.aux
	rm -fr linkingto-proposal.out
	rm -fr linkingto-proposal.log
	
