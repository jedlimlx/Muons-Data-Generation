import subprocess
import threading
import random
import numpy as np
import pandas as pd

import os
import sys
import tqdm

RESOLUTION = 64
SCALE = 2
SENSOR_DIST = 10
N = 10000
N_threads = 6
FILE_PATH = sys.path[0]


def run_threads(run, i, voxels):
    # Running the program
    bins = np.arange(-12*SCALE, 12*SCALE + 12*SCALE/RESOLUTION, 24*SCALE/RESOLUTION)
    np.savetxt("run_" + str(i) + ".voxel", voxels.flatten(), delimiter=" ", fmt="%1d")
    args = ["mu", "run.mac", "run_" + str(i)]
    proc = subprocess.Popen(args=[os.path.join(FILE_PATH, "mu"), "run.mac", "run_" + str(i)], stdout=subprocess.DEVNULL)
    proc.wait()
    # print(i)

    # Converting into map of number of muons
    txt_df = pd.read_csv(
        "run_" + str(i) + ".txt",
        delimiter=" ", header=None,
        names=[
            "event",
            "count",
            "x",
            "y",
            "z",
            "px",
            "py",
            "pz",
            "ver_x",
            "ver_y",
            "ver_z",
            "ver_px",
            "ver_py",
            "ver_pz",
            "time",
            "eIn",
            "eDep",
            "trackID",
            "copyNo",
            "particleID"
        ]
    )
    txt_df.to_csv("raw_detections/run_" + str(run) + "_orient_" + str(i) + ".csv")

    # Calculating output detector planes
    output_txt_df = txt_df[(txt_df["particleID"] == 13) & (txt_df["x"] > 150*SCALE)][["y", "z", "px", "py", "pz", "count"]]

    # Compute 1st plane
    output_txt_df["y_cut"] = pd.cut(output_txt_df["y"], bins=bins, right=False)
    output_txt_df["z_cut"] = pd.cut(output_txt_df["z"], bins=bins, right=False)
    pt = pd.pivot_table(output_txt_df, columns="y_cut", index="z_cut", values="count", aggfunc="sum")
    np.save("detections/" + str(run) + "_orient_" + str(i) + "_0.npy", pt.values)

    # Compute ith plane
    for j in range(1, 20):
        t = SENSOR_DIST * j / txt_df["px"]
        output_txt_df["y2"] = output_txt_df["y"] + output_txt_df["py"] * t
        output_txt_df["z2"] = output_txt_df["z"] + output_txt_df["pz"] * t

        min_y, max_y = np.min(output_txt_df["y"].values), np.max(output_txt_df["y"].values)
        min_z, max_z = np.min(output_txt_df["z"].values), np.max(output_txt_df["z"].values)
        txt_df_2 = output_txt_df[
            (min_y < output_txt_df["y2"]) & (max_y > output_txt_df["y2"]) & (min_z < output_txt_df["z2"]) & (max_z > output_txt_df["y2"])
        ][["y2", "z2", "count"]]

        txt_df_2["y2_cut"] = pd.cut(txt_df_2["y2"], bins=bins, right=False)
        txt_df_2["z2_cut"] = pd.cut(txt_df_2["z2"], bins=bins, right=False)
        pt = pd.pivot_table(txt_df_2, columns="y2_cut", index="z2_cut", values="count", aggfunc="sum")
        np.save("detections/" + str(run) + "_orient_" + str(i) + f"_{j}.npy", pt.values)

    # Calculating input detector planes
    input_txt_df = txt_df[(txt_df["particleID"] == 13) & (txt_df["x"] > 150*SCALE)][["ver_y", "ver_z", "ver_px", "ver_py", "ver_pz", "count"]]

    # Compute 1st plane
    input_txt_df["y_cut"] = pd.cut(txt_df["ver_y"], bins=bins, right=False)
    input_txt_df["z_cut"] = pd.cut(txt_df["ver_z"], bins=bins, right=False)
    pt = pd.pivot_table(input_txt_df, columns="y_cut", index="z_cut", values="count", aggfunc="sum")
    np.save("detections/" + str(run) + "_orient_" + str(i) + "_0_input.npy", pt.values)

    # Compute ith plane
    for j in range(1, 20):
        t = -SENSOR_DIST * j / txt_df["ver_px"]
        input_txt_df["y2"] = input_txt_df["ver_y"] + input_txt_df["ver_py"] * t
        input_txt_df["z2"] = input_txt_df["ver_z"] + input_txt_df["ver_pz"] * t

        min_y, max_y = np.min(input_txt_df["ver_y"].values), np.max(input_txt_df["ver_y"].values)
        min_z, max_z = np.min(input_txt_df["ver_z"].values), np.max(input_txt_df["ver_z"].values)
        txt_df_2 = input_txt_df[
            (min_y < input_txt_df["y2"]) & (max_y > input_txt_df["y2"]) & (min_z < input_txt_df["z2"]) & (max_z > input_txt_df["y2"])
        ][["y2", "z2", "count"]]

        txt_df_2["y2_cut"] = pd.cut(txt_df_2["y2"], bins=bins, right=False)
        txt_df_2["z2_cut"] = pd.cut(txt_df_2["z2"], bins=bins, right=False)
        pt = pd.pivot_table(txt_df_2, columns="y2_cut", index="z2_cut", values="count", aggfunc="sum")
        np.save("detections/" + str(run) + "_orient_" + str(i) + f"_{j}_input.npy", pt.values)


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
    threads = []
    for j in tqdm.trange(N):
        voxels = np.zeros((RESOLUTION, RESOLUTION, RESOLUTION), dtype=np.int32)

        # Materials (Fe, Ni, Sn, Ag, Pb, Au, U)
        lst = list(range(1, 8))
        radiation_lengths = [1.757, 1.424, 1.206, 0.8543, 0.5612, 0.3344, 0.3166]

        # Converting to voxels
        random.shuffle(lst)
        for i in lst[:random.randint(1, 3)]:
            voxels = generate_voxels(i, voxels)

        # Running simulation
        orientations = rotate_cube(voxels)
        for i in range(N_threads):
            th = threading.Thread(target=run_threads, args=(j, i, orientations[i]))
            th.start()
            threads.append(th)

        for th in threads:
            th.join()

        np.save("voxels/run_" + str(j) + ".npy", voxels)
        # print("Run", j)


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
