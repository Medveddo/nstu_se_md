```shell
docker build -t par_lab2 . && docker run -it --rm -v %cd%\inner:/usr/src/app/inner/ par_lab2
```