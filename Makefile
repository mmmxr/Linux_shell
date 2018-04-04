.PHONY:clean

shell:shell.c
	gcc $^ -o $@

clean:
	rm shell

