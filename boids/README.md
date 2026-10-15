# boids

The boids! -Flowery (incorrect quote)

# usage

Sorry to keep you waiting.

```
Boids::Boids boids;
for (int i = 0; i < 64; i++) boids.add({Boids::rand01() * 2 - 1, Boids::rand01() * 2 - 1});
for (int i = 0; i < (1<<12); i++) boids.update();
Boids::animate(boids, 1024, "video");
```