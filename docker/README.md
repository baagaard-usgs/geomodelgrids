# Docker images

```
# Run in top-level source directory
bash> docker build -t $TAG -f docker/$FILE .
```

```
bash> docker run --rm -it -v $TOOLS_DIR/earthvision-11:/opt/earthvision -v $HOME/data/cencal-geology:/data/cencal-geology baagaard/earthvision-env /bin/bash
```
