#!/usr/bin/env python3

import sys
import colorsys


def main():
    text = sys.argv[1]

    for i, c in enumerate(text):
        hue = i / len(text)
        rgb = colorsys.hsv_to_rgb(hue, 1, 1)
        print("/bc%02x%02x%02x/%s" % (int(rgb[0] * 255), int(rgb[1] * 255), int(rgb[2] * 255), c), end="")

    print()


if __name__ == "__main__":
    main()
