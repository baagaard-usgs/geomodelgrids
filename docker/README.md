# Docker images

```console
# Run in top-level source directory
docker build -t $TAG -f docker/$FILE .
```

```
docker run --rm -it -v $TOOLS_DIR/earthvision-11:/opt/earthvision -v $HOME/data/cencal-geology:/data/cencal-geology baagaard/earthvision-env /bin/bash
```
