# FIRE

https://fabiensanglard.net/doom_fire_psx/

# USAGE

```
DoomFire fire(256, 128, time(0));
fire.SRC = 64;
fire.RIGHT_MARGIN = 0;
fire.defaultSources();

fire.clear();

for (int i = 0; i < 67; i++) {
	fire.update();
	std::cout << fire.buffer() << "\n";
}
```