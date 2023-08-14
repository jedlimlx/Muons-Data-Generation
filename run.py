import os
import tqdm
import random
import threading
import subprocess
import numpy as np
import tensorflow as tf


RESOLUTION = 64


def rotate_cube(cuberay):
    res = []
    res.append(cuberay)
    res.append(np.rot90(cuberay, 2, axes=(0, 2)))
    res.append(np.rot90(cuberay, axes=(0, 2)))
    res.append(np.rot90(cuberay, -1, axes=(0, 2)))
    res.append(np.rot90(cuberay, axes=(0, 1)))
    res.append(np.rot90(cuberay, -1, axes=(0, 1)))
    return res


def _f(t):
    return t*t*t*(t*(t*6 - 15) + 10)


def generate_perlin_noise_2d(batch_size, shape, res):
    delta = (res[0] / shape[0], res[1] / shape[1])
    d = (shape[0] // res[0], shape[1] // res[1])
    grid = tf.meshgrid(tf.range(0, res[0], delta[0]),
                       tf.range(0, res[1], delta[1]), indexing='ij')
    grid = tf.stack(grid, axis=-1)
    grid = grid - tf.floor(grid)
    grid = tf.cast(grid, tf.float32)

    angles = tf.random.uniform(shape=(batch_size, res[0] + 1, res[1] + 1), maxval=2 * np.pi)
    gradients = tf.stack((tf.cos(angles), tf.sin(angles)), axis=-1)

    # print(d)

    gradients = tf.repeat(tf.repeat(gradients, repeats=d[0], axis=1), repeats=d[1], axis=2)
    g00 = gradients[:, :-d[0], :-d[1]]
    g10 = gradients[:, d[0]:, :-d[1]]
    g01 = gradients[:, :-d[0], d[1]:]
    g11 = gradients[:, d[0]:, d[1]:]
    # Ramps
    n00 = tf.reduce_sum(tf.stack((grid[:, :, 0], grid[:, :, 1]), axis=-1) * g00, axis=3)
    n10 = tf.reduce_sum(tf.stack((grid[:, :, 0] - 1, grid[:, :, 1]), axis=-1) * g10, axis=3)
    n01 = tf.reduce_sum(tf.stack((grid[:, :, 0], grid[:, :, 1] - 1), axis=-1) * g01, axis=3)
    n11 = tf.reduce_sum(tf.stack((grid[:, :, 0] - 1, grid[:, :, 1] - 1), axis=-1) * g11, axis=3)
    # Interpolation
    t = _f(grid)
    n0 = n00 * (1 - t[:, :, 0]) + t[:, :, 0] * n10
    n1 = n01 * (1 - t[:, :, 0]) + t[:, :, 0] * n11
    return 6.21908435118 * ((1 - t[:, :, 1]) * n0 + t[:, :, 1] * n1)  # for a std dev of 1


def generate_perlin_noise_3d(shape, res):
    """
    Generates 3D perlin noise image
    Args:
        shape: output shape
        res: resolution of Perlin noise grid

    Returns:
        3D image
    """

    delta = (res[0] / shape[0], res[1] / shape[1], res[2] / shape[2])
    d = (shape[0] // res[0], shape[1] // res[1], shape[2] // res[2])
    grid = tf.meshgrid(tf.range(0, res[0], delta[0]),
                       tf.range(0, res[1], delta[1]),
                       tf.range(0, res[2], delta[2]), indexing="ij")
    grid = tf.stack(grid, axis=-1)
    grid = grid - tf.floor(grid)
    grid = tf.cast(grid, tf.float32)
    t = _f(grid)
    # Gradients
    theta = 2 * np.pi * tf.random.uniform(shape=[res[0], res[1] + 1, res[2] + 1])
    phi = 2 * np.pi * tf.random.uniform(shape=[res[0], res[1] + 1, res[2] + 1])
    gradients = tf.stack((tf.sin(phi) * tf.cos(theta), tf.sin(phi) * tf.sin(theta), tf.cos(phi)), axis=3)
    gradients = tf.concat([gradients, tf.expand_dims(gradients[0], 0)], axis=0)

    g000 = tf.repeat(tf.repeat(tf.repeat(gradients[0:-1, 0:-1, 0:-1], d[0], 0), d[1], 1), d[2], 2)
    n000 = tf.reduce_sum(tf.stack((grid[:, :, :, 0], grid[:, :, :, 1], grid[:, :, :, 2]), axis=3) * g000, 3)
    g100 = tf.repeat(tf.repeat(tf.repeat(gradients[1:, 0:-1, 0:-1], d[0], 0), d[1], 1), d[2], 2)
    n100 = tf.reduce_sum(tf.stack((grid[:, :, :, 0] - 1, grid[:, :, :, 1], grid[:, :, :, 2]), axis=3) * g100, 3)
    n00 = n000 * (1 - t[:, :, :, 0]) + t[:, :, :, 0] * n100

    g010 = tf.repeat(tf.repeat(tf.repeat(gradients[0:-1, 1:, 0:-1], d[0], 0), d[1], 1), d[2], 2)
    n010 = tf.reduce_sum(tf.stack((grid[:, :, :, 0], grid[:, :, :, 1] - 1, grid[:, :, :, 2]), axis=3) * g010, 3)
    g110 = tf.repeat(tf.repeat(tf.repeat(gradients[1:, 1:, 0:-1], d[0], 0), d[1], 1), d[2], 2)
    n110 = tf.reduce_sum(tf.stack((grid[:, :, :, 0] - 1, grid[:, :, :, 1] - 1, grid[:, :, :, 2]), axis=3) * g110, 3)
    n10 = n010 * (1 - t[:, :, :, 0]) + t[:, :, :, 0] * n110

    n0 = (1 - t[:, :, :, 1]) * n00 + t[:, :, :, 1] * n10

    g001 = tf.repeat(tf.repeat(tf.repeat(gradients[0:-1, 0:-1, 1:], d[0], 0), d[1], 1), d[2], 2)
    n001 = tf.reduce_sum(tf.stack((grid[:, :, :, 0], grid[:, :, :, 1], grid[:, :, :, 2] - 1), axis=3) * g001, 3)
    g101 = tf.repeat(tf.repeat(tf.repeat(gradients[1:, 0:-1, 1:], d[0], 0), d[1], 1), d[2], 2)
    n101 = tf.reduce_sum(tf.stack((grid[:, :, :, 0] - 1, grid[:, :, :, 1], grid[:, :, :, 2] - 1), axis=3) * g101, 3)
    n01 = n001 * (1 - t[:, :, :, 0]) + t[:, :, :, 0] * n101

    g011 = tf.repeat(tf.repeat(tf.repeat(gradients[0:-1, 1:, 1:], d[0], 0), d[1], 1), d[2], 2)
    n011 = tf.reduce_sum(tf.stack((grid[:, :, :, 0], grid[:, :, :, 1] - 1, grid[:, :, :, 2] - 1), axis=3) * g011, 3)
    g111 = tf.repeat(tf.repeat(tf.repeat(gradients[1:, 1:, 1:], d[0], 0), d[1], 1), d[2], 2)
    n111 = tf.reduce_sum(tf.stack((grid[:, :, :, 0] - 1, grid[:, :, :, 1] - 1, grid[:, :, :, 2] - 1), axis=3) * g111, 3)
    n11 = n011 * (1 - t[:, :, :, 0]) + t[:, :, :, 0] * n111

    n1 = (1 - t[:, :, :, 1]) * n01 + t[:, :, :, 1] * n11

    return (1 - t[:, :, :, 2]) * n0 + t[:, :, :, 2] * n1 + 0.5


@tf.function
def generate_fractal_noise_2d(batch_size, shape, res, init_frequency=1, octaves=1, persistence=0.5):
    noise = tf.zeros(shape=shape)
    frequency = init_frequency
    amplitude = 1.0
    scaling = 0
    for _ in range(octaves):
        scaling += amplitude
        noise += amplitude * generate_perlin_noise_2d(batch_size, shape, (frequency * res[0], frequency * res[1]))
        frequency *= 2
        amplitude *= persistence

    return noise / scaling


def generate_fractal_noise_3d(shape, res, octaves=1, persistence=0.5):
    """
    Generates Perlin noise of different frequencies
    Args:
        shape: shape of output image
        res: resolution of Perlin noise of the lowest frequency
        octaves: number of different frequencies'
        persistence: amount to decrease amplitude by after frequency increases

    Returns:
        3D image
    """
    noise = tf.zeros(shape=shape)
    frequency = 1
    amplitude = 1.0
    scaling = 0
    for _ in range(octaves):
        scaling += amplitude
        noise += amplitude * generate_perlin_noise_3d(shape,
                                                      (frequency * res[0], frequency * res[1], frequency * res[2]))
        frequency *= 2
        amplitude *= persistence

    return noise * 0.5 / scaling


def thread_function(thread_num, i, voxels):
    np.savetxt(f"voxels_{thread_num}.txt", voxels.flatten().astype(np.int32), delimiter="\n", fmt="%d")
    np.save("voxels/run_" + str(i) + ".npy", voxels)

    # Running the simulation
    proc = subprocess.Popen(args=[
        "./build/mu", "1",
        f"voxels_{thread_num}.txt",
        f"output/run_{i}.csv"
    ], stdout=subprocess.DEVNULL)
    proc.wait()


if __name__ == "__main__":
    num_threads = 12

    # Copying the macro files
    os.system("cp macros/* ./")

    i = 2400
    pbar = tqdm.tqdm(total=10000)
    while i < 10000:
        threads = []
        for j in range(num_threads):
            """
            voxels = np.zeros((RESOLUTION, RESOLUTION, RESOLUTION), dtype=np.int32)

            # List of materials by radiation length
            # Materials (Concrete, Al, Ti, Fe, Ni, Sn, Ag, Pb, Au, U)
            radiation_lengths = [11.55, 8.897, 3.560, 1.757, 1.424, 1.206, 0.8543, 0.5612, 0.3344, 0.3166]

            # Converting to voxels
            num = random.randint(1, 4)
            for k in range(num):
                value = random.randint(1, 10)
                threshold = random.uniform(0.7 + 0.1 * num + 0.01 * value, 1.3 + 0.1 * num + 0.01 * value)

                noise = generate_fractal_noise_3d((RESOLUTION, RESOLUTION, RESOLUTION), (2, 2, 2), octaves=3)
                noise = (noise - tf.math.reduce_mean(noise)) / tf.math.reduce_std(noise)
                voxels = voxels + value * ((noise.numpy() > threshold) & (voxels == 0)).astype("int32")
                # print(voxels)
            """

            voxels = np.load(f"voxels/run_{i}.npy")

            # Running thread
            thread = threading.Thread(target=thread_function, args=(j, i, voxels))
            thread.start()
            threads.append(thread)

            i += 1

        for thread in threads:
            thread.join()
            pbar.update(1)

    # Deleting the macro files
    os.system("rm run.mac vis.mac")
