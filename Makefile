TARGET=cs311_1
ALL: $(TARGET) $(TARGET)_timing.log $(TARGET).pdf

$(TARGET): $(TARGET).c
	gcc -O3 -o $(TARGET) $(TARGET).c

$(TARGET)_timing.log: $(TARGET)
	> $(TARGET).log
	buflen=1 ; while [ $$buflen -le 8192 ] ; do \
		if [ ! -f timing_$$buflen.log ] ; then \
			$(TARGET) -o -b $$buflen /usr/local/classes/eecs/winter2012/cs311/data_to_read data_to_write > timing_$$buflen.log ; \
		fi ; \
		cat timing_$$buflen.log >> $(TARGET)_timing.log ; \
		((buflen = buflen \* 2)) ; \
	done

$(TARGET).dvi: $(TARGET).tex 
	latex $(TARGET).tex
	latex $(TARGET).tex

$(TARGET).ps: $(TARGET).dvi
	dvips -R -Poutline -t letter $(TARGET).dvi -o $(TARGET).ps

$(TARGET).pdf: $(TARGET).ps
	ps2pdf $(TARGET).ps

