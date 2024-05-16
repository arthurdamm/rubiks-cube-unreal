import numpy as np

def rotation_matrix(axis, theta):
    """
    Generate a rotation matrix for rotating `theta` radians around `axis`.
    """
    axis = np.asarray(axis)
    axis = axis / np.sqrt(np.dot(axis, axis))
    a = np.cos(theta / 2.0)
    b, c, d = -axis * np.sin(theta / 2.0)

    aa, bb, cc, dd = a * a, b * b, c * c, d * d
    bc, ad, ac, ab, bd, cd = b * c, a * d, a * c, a * b, b * d, c * d
    return np.array([[aa + bb - cc - dd, 2 * (bc + ad), 2 * (bd - ac)],
                     [2 * (bc - ad), aa + cc - bb - dd, 2 * (cd + ab)],
                     [2 * (bd + ac), 2 * (cd - ab), aa + dd - bb - cc]])

def apply_rotation(pos, rot_center, rotation_matrix):
    """
    Apply a rotation to a position `pos` using a specified `rotation_matrix` and a `rot_center`.
    """
    rel_pos = pos - rot_center  # Translate to origin
    rotated_pos = np.dot(rotation_matrix, rel_pos)  # Rotate
    final_pos = rotated_pos + rot_center  # Translate back
    return final_pos

# Define rotation center and initial position
rotation_center = np.array([0, 0, 0])
initial_position = np.array([1, 0, 0])  # Example position

# 90 degrees rotation around the z-axis
theta = -np.pi / 2  # 90 degrees in radians
rotation_axis = [0, 0, 1]  # Rotating around z-axis

# Get the rotation matrix
rot_matrix = rotation_matrix(rotation_axis, theta)

# Apply the rotation
print("Initial Position:", initial_position)

rotated_position = apply_rotation(initial_position, rotation_center, rot_matrix)
print("Rotated Position:", rotated_position)
rotated_position = apply_rotation(rotated_position, rotation_center, rot_matrix)
print("Rotated Position:", rotated_position)
rotated_position = apply_rotation(rotated_position, rotation_center, rot_matrix)
print("Rotated Position:", rotated_position)
rotated_position = apply_rotation(rotated_position, rotation_center, rot_matrix)
print("Rotated Position:", rotated_position)




