# OnkoC Medical Screening

Modularni CLI sustav za upravljanje onkološkim screeningom pacijenata uz SQLite bazu podataka.

## Ključne funkcionalnosti

- SQLite sloj (`pacijenti`, `pracenje`, `audit_log`) s integritetom relacija
- Siguran unos podataka bez `scanf` overflow rizika (`fgets` + validacija)
- CRUD operacije za pacijente
- Pretraga pacijenata (ID, ime/prezime, tip screeninga)
- Sortiranje (ime, rizik, datum) i paginacija rezultata
- Statistika (ukupno pacijenata, prosječan rizik po tipu)
- Izvoz u CSV i backup baze
- Audit log operacija i `undo` zadnjeg brisanja
- Poruke uspjeha/pogreške s vremenskom oznakom

## Struktura modula

- `main.c` – pokretanje aplikacije i inicijalizacija baze
- `database.c/.h` – SQLite operacije i schema
- `validation.c/.h` – validacija unosa
- `utils.c/.h` – pomoćne funkcije (siguran unos, timestamp poruke, potvrde)
- `ui.c/.h` – korisnički izbornik i tokovi operacija

## Build i pokretanje

```bash
make
./onkoc
```

## Čišćenje artefakata

```bash
make clean
```
