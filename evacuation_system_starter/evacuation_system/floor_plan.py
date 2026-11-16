"""Approximate room geometry based on the hand-drawn plan.

Coordinates are in metres. This is a first visualization draft, not a
survey-accurate architectural drawing. Refine polygons when exact wall,
door and corridor measurements are confirmed.
"""
ROOM_POLYGONS = {
    # Main rooms, left to right in the corrected image.
    "A330": [(0, 0), (12.5, 0), (12.5, 10.3), (0, 10.3)],
    "A331": [(12.5, 0), (23.0, 0), (23.0, 12.5), (12.5, 10.3)],
    # A332 has a visible notch along the corridor-facing boundary.
    "A332": [
        (23.0, 0), (34.0, 0), (34.0, 9.7),
        (30.0, 9.7), (30.0, 7.8), (23.0, 7.8)
    ],
    "A333": [(34.0, 0), (41.5, 0), (41.5, 12.8), (34.0, 12.8)],
    # A329 is sketched as a separate corridor-side area; dimensions are not confirmed.
    "A329": [(5.0, -4.9), (15.0, -4.9), (15.0, -0.2), (5.0, -0.2)],
}

ROOM_LABEL_POSITIONS = {
    "A330": (6.25, 5.15),
    "A331": (17.75, 6.0),
    "A332": (27.0, 4.8),
    "A333": (37.75, 6.4),
    "A329": (10.0, -2.5),
}

# Corridor and stair blocks are schematic placeholders.
CORRIDOR_POLYGON = [
    (0, -0.2), (41.5, -0.2), (41.5, -2.2),
    (30.0, -2.2), (23.0, -3.0), (15.0, -2.2),
    (5.0, -2.2), (0, -2.2)
]
STAIR_POLYGONS = {
    "Stair west": [(-1.0, -9.0), (3.0, -9.0), (3.0, -2.2), (-1.0, -2.2)],
    "Stair east": [(38.0, -9.0), (42.0, -9.0), (42.0, -2.2), (38.0, -2.2)],
}
