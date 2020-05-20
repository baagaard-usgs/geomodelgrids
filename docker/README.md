# Docker images

```
# Run in top-level source directory
bash> docker build -t $TAG -f docker/$FILE .
```

```
bash> docker run -it -v $TOOLS_DIR/earthvision-10:/opt/earthvision -v $HOME/data/cencal-geology:/data/cencal-geology earthvision-env /bin/bash
```
