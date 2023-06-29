import tqdm
import numpy as np


RESOLUTION = 128


def rotate_cube(cuberay):
    res = []
    res.append(cuberay)
    res.append(np.rot90(cuberay, 2, axes=(0, 2)))
    res.append(np.rot90(cuberay, axes=(0, 2)))
    res.append(np.rot90(cuberay, -1, axes=(0, 2)))
    res.append(np.rot90(cuberay, axes=(0, 1)))
    res.append(np.rot90(cuberay, -1, axes=(0, 1)))
    return res


def generate_voxels(value, voxels):
    noise = PerlinNoise()
    return np.array([[[
        value if noise.noise(
            x / RESOLUTION,
            y / RESOLUTION,
            z / RESOLUTION
        ) > 0.6 else voxels[x, y, z] for x in range(RESOLUTION)
    ] for y in range(RESOLUTION)] for z in range(RESOLUTION)])


def main():
    for j in tqdm.trange(1):
        voxels = np.zeros((RESOLUTION, RESOLUTION, RESOLUTION), dtype=np.int32)

        # Converting to voxels
        voxels = generate_voxels(1, voxels)
        np.savetxt("voxel.voxel", voxels.flatten(), delimiter="\n", fmt="%1d")
        # np.save("voxels/run_" + str(j) + ".npy", voxels)


def fade(t):
    return t * t * t * (t * (t * 6 - 15) + 10)


def lerp(t, a, b):
    return a + t * (b - a)


def grad(ahash, x, y, z):
    # very screwed up bitwise operations
    h = ahash & 15
    u = x if h < 8 else y
    v = y if h < 4 else x if (h == 12 or h == 14) else z
    return (u if (h & 1) == 0 else -u) + (v if (h & 2) == 0 else -v)


class PerlinNoise: # because the library has "undesired behaviour"
    def __init__(self, seed=None):
        if seed is None:
            p = np.arange(256)
            np.random.shuffle(p)
            self.p = np.tile(p, 2)
        else:
            np.random.seed(seed)
            p = np.arange(256)
            np.random.shuffle(p)
            self.p = np.tile(p, 2)

    def noise(self, x, y, z): # i have no idea what this does
        X = int(np.floor(x)) & 255
        Y = int(np.floor(y)) & 255
        Z = int(np.floor(z)) & 255

        x -= np.floor(x)
        y -= np.floor(y)
        z -= np.floor(z)

        u = fade(x)
        v = fade(y)
        w = fade(z)

        A = self.p[X] + Y
        AA = self.p[A] + Z
        AB = self.p[A + 1] + Z
        B = self.p[X + 1] + Y
        BA = self.p[B] + Z
        BB = self.p[B + 1] + Z

        # ???
        res = lerp(w, lerp(v, lerp(u, grad(self.p[AA], x, y, z), grad(self.p[BA], x-1, y, z)), lerp(u, grad(self.p[AB], x, y-1, z), grad(self.p[BB], x-1, y-1, z))),	lerp(v, lerp(u, grad(self.p[AA+1], x, y, z-1), grad(self.p[BA+1], x-1, y, z-1)), lerp(u, grad(self.p[AB+1], x, y-1, z-1),	grad(self.p[BB+1], x-1, y-1, z-1))))
        return (res + 1.0)/2.0


if __name__ == "__main__":
    main()
