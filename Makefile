my_ls: apply.c compare.c create.c destroy.c handle_args.c info.c main.c options.c print.c sort.c update.c
	gcc -g -o my_ls main.c apply.c compare.c create.c destroy.c print.c update.c handle_args.c info.c options.c sort.c -o my_ls