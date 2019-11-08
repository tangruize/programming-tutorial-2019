FLAGS = -file-line-error -synctex=1 -interaction=nonstopmode

DIR ?=
VIEWER ?=

CMD_SUFFIX = &
ifeq ($(DIR),)
	AUTODIR = $(wildcard 201*)
else
	AUTODIR = $(DIR)
	ifeq ($(VIEWER),)
		ifeq ($(OS),Windows_NT)
			CMD_PREFIX = cmd /c start /B
			VIEWER = SumatraPDF.exe
			CMD_SUFFIX = ;
		else
			VIEWER = evince
		endif
	endif
endif

TEX = $(filter-out $(wildcard $(AUTODIR:%=%/preamble.tex)),$(wildcard $(AUTODIR:%=%/*.tex)))
PDF = $(TEX:.tex=.pdf)

slides: $(PDF)
	@if which "$(VIEWER)" &> /dev/null; then $(CMD_PREFIX) $(VIEWER) $(PDF) $(CMD_SUFFIX) fi

$(PDF): %.pdf: %.tex
	@cd $(dir $@) && latexmk -use-make -silent -xelatex $(FLAGS) $(notdir $(@:.pdf=.tex))

clean:
	@find $(AUTODIR) -type f ! -name \*.tex -a ! -name \*.bib -a ! -name \*.png -a ! -name Makefile -delete

.DEFAULT_GOAL := slides
.PHONY: $(PDF)
