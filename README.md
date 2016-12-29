# Memcached client

## Budowanie:
```
make all
```

## Uruchamianie
```
./mc_client [ip] [port] [plik do pobrania] [plik do zapisania]
```
np.
```
./mc_client 127.0.0.1 1234 file1.jpg files/downloaded.jpg
```

Można pominąć [plik do zapisania], wtedy plik zostanie zapisany w aktualnym katalogu.