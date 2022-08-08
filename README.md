#### CCanny

A simple (naive) implementation for canny algorithm in C.

#### Run

1. ```shell
   chmod a+x run.sh
   ```

2. ```shell
   ./a.out -f ./data/tanjirou.jpg -b 3 -s 5 -l 20 -h 50 -i
   ```

   Parameters:

   - -f: Path to input image.
   - -b: Size of blur kernel.
   - -s: Size of sobel kernel $\in\{3,5\}$.
   - -l: Low threshold of canny $\in[0,255]$.
   - -h: High threshold of canny $\in[t_{min},255]$.
   - -i: Decide whether to show internal results.

#### Data Provided

1. `data/girl.jpg`: My ubuntu's wallpaper.
2. `data/nezuko.jpg`:  A cute girl.
3. `data/tanjirou.jpg`: A cool guy.

