all: evc rbc

test: test.c locationManager.c
	gcc test.c locationManager.c -o test

evc: evc.c messagerie.c locationManager.c
	gcc evc.c messagerie.c locationManager.c -o evc

rbc: rbc.c messagerie.c locationManager.c
	gcc rbc.c messagerie.c locationManager.c -o rbc
