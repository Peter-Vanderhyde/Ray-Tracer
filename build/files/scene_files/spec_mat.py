

def create_text(filename):
    with open(filename, 'w') as f:
        materials = []
        textures = []
        spheres = []
        for reflect in range(11):
            for gloss in range(11):
                mat_string = f"material spec{reflect * 11 + gloss} specular {round(reflect * 0.1, 1)} {round(gloss * 0.1, 1)}\n"
                tex_string = f"texture spec_tex{reflect * 11 + gloss} specular_texture red spec{reflect * 11 + gloss}\n"
                x = (gloss - 5)
                y = (reflect - 5) * -1
                sphere_string = f"sphere ({x} {y} 0.5) 0.5 (1 1) (0 0 0) spec{reflect * 11 + gloss} spec_tex{reflect * 11 + gloss}\n"
                materials.append(mat_string)
                textures.append(tex_string)
                spheres.append(sphere_string)
        
        file_lines = [
            "texture red solid (1 0 0)\n",
            "texture white solid (1 1 1)\n",
            "material floor diffuse\n",
        ]
        file_lines += materials
        file_lines += textures
        file_lines += spheres
        file_lines += [
            "\n",
            "sky true\n",
            "sun (-10 -10 -25) 20 (1 1 1) 2\n",
            "rays 10 10\n",
            "pixels 600 600\n",
            "camera (0 0 10) (0 0 0) (0 1 0) 70\n",
            "output spec_test.png\n"
        ]
        f.writelines(file_lines)


create_text("matte.txt")