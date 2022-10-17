# Evaluatie project systeemprogrammeren **deel 1**

Het project wordt voornamelijk geëvalueerd op welke deel-functionaliteiten
correct werken.

Met onderstaande lijst kan makkelijk nagegaan worden wat er nog ontbreekt aan
het project. Als alle functionaliteiten in de lijst correct voorzien zijn, leidt
dit normaal tot een zeer hoge of maximale score voor het project.

Er kunnen echter ook punten verloren worden, zie hiervoor de lijst helemaal onderaan.

Deze lijst is louter informatief, en kan nog verduidelijkt en/of aangevuld worden.
Indien de opgave iets vraagt wat niet in de lijst staat, moet dat zeker ook gedaan worden!
Meld dit dan dus zeker, zodat we deze lijst op tijd kunnen aanpassen.
Ook bij onduidelijkheden neem je best contract op.



Functionaliteit:
- [ ] `spg_part1` met 0 command line arguments (= alle fixed levels)

- [ ] `spg_part1` met command line argument `--help`
- [ ] `spg_part1` met 1 of meerdere numerieke command line level arguments
- [ ] `spg_part1` met 1 of meerdere filename command line level arguments
- [ ] `spg_part1` met `demo` als command line level argument (case insensitive)
- [ ] `spg_part1` met meerdere command line level argumenten, met mix van de 3 types door elkaar
- [ ] `spg_part1` met command line argument `--`
- [ ] `spg_edit` met enkel command line argument
- [ ] `spg_edit` met 0, 2 of meer command line arguments (= foutmelding)
- [ ] `spg_edit` met command line argument `--help`


- [ ] Binaire levels inlezen (met ondersteuning "same")

- [ ] Spelregels: Selecteren van te besturen auto
- [ ] Spelregels: Basis Beweging auto's
- [ ] Spelregels: Zwaartekracht
- [ ] Spelregels: Correcte snelheid beweging
- [ ] Spelregels: Markers weergegeven
- [ ] Spelregels: beweging beperkt tot wegen
- [ ] Spelregels: beweging geblokkeerd door filters
- [ ] Spelregels: Crate oprapen en laten vallen (functioneel)
- [ ] Spelregels: Crate oprapen en laten vallen (visueel)
- [ ] Spelregels: Filters activeren/deactiveren bij juiste aantal crates
- [ ] Spelregels: Uitgang
- [ ] Spelregels: Basis automatische beweging autos (op zijn minst altijd vooruit rijden)
- [ ] Spelregels: Correcte automatische beweging autos voor deel 1 (rechterhand regel)
- [ ] Level editor: Juiste level bestand inlezen voor numerieke levels (`*_edited.lvl` indien beschikbaar)
- [ ] Level editor: Selecteren functie via toetsen
- [ ] Level editor: Uitvoeren toevoeg functie omgeving (gras, water, weg, gebouw)
- [ ] Level editor: Uitvoeren toevoeg functie objecten (auto, uitgang, crate, marker, flora, filter)
- [ ] Level editor: Uitvoeren aanpas functies (roteer, kleur, verwijder)
- [ ] Level editor: Uitvoeren impliciete aanpas functies bij klikken bestaande object (hoogte gebouw, nummer filter, rotatie uitgang, kleur auto, kleur crate, type flora, kleur marker)
- [ ] Level editor: Entities omzetten naar `struct Level`
- [ ] Level editor: Opslaan levels juiste naar bestand (`*_edited.lvl`)
- [ ] Binaire levels schrijven (met ondersteuning "same")

Aftrek voor fouten:
- Crashes
- Memory leaks
- Compiled niet op Raspberry Pi
- Compiled niet op Linux
- Performantie problemen
- Zeer "slechte" code
- Niet in C (deel 1)
- Niet voor elke sprint een startverslagen en eindverslagen (of niet op tijd)
- Geen eindverslag in [REPORT_1.md](REPORT_1.md)
- Opgave niet op tijd ingediend
- Andere fouten