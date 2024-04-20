import subprocess
import glob


# compilation command example
# /Users/username/VulkanSDK/1.3.250.1/macOS/bin/MoltenVKShaderConverter -gi fragment.glsl -so fragment.spv -t f

compiler_path = '/Users/takuyamusha/VulkanSDK/1.3.250.1/macOS/bin/MoltenVKShaderConverter'


def compile_shader(path, isVertexShader, isComputeShader):

    arg1 = '-gi'
    shader_input = path
    arg2 = '-so'
    shader_output = 'Shaders/spv/' + path.split('.')[0].split('/')[-1] + 'Frag.spv'
    if isVertexShader:
        shader_output = 'Shaders/spv/' + path.split('.')[0].split('/')[-1] + 'Vert.spv'
    if isComputeShader:
        shader_output = 'Shaders/spv/' + path.split('.')[0].split('/')[-1] + 'Comp.spv'
    arg3 = '-t'
    arg4 = 'f'
    if isVertexShader:
        arg4 = 'v'
    if isComputeShader:
        arg4 = 'c'

    result = subprocess.run([compiler_path, arg1,shader_input, arg2, shader_output, arg3, arg4], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
    print(result.stdout.decode('utf-8'))



frag_shaders = glob.glob(f"Shaders/*.frag")
vert_shaders = glob.glob(f"Shaders/*.vert")
comp_shaders = glob.glob(f"Shaders/*.comp")
frag_sample_shaders = glob.glob(f"src/Samples/*/*.frag")
vert_sample_shaders = glob.glob(f"src/Samples/*/*.vert")
comp_sample_shaders = glob.glob(f"src/Samples/*/*.comp")


for path in frag_shaders:
    compile_shader(path, False, False)

for path in vert_shaders:
    compile_shader(path, True, False)

for path in comp_shaders:
    compile_shader(path, False, True)

for path in frag_sample_shaders:
    compile_shader(path, False, False)

for path in vert_sample_shaders:
    compile_shader(path, True, False)

for path in comp_sample_shaders:
    compile_shader(path, False, True)

print("done")

"""
arg1 = '-gi'
shader_input = 'PP_Sepia.frag'
arg2 = '-so'
shader_output = 'PP_SepiaFrag.spv'
arg3 = '-t'
arg4 = 'f'

result = subprocess.run([compiler_path, arg1,shader_input, arg2, shader_output, arg3, arg4], stdout=subprocess.PIPE, stderr=subprocess.PIPE)
print(result.stdout.decode('utf-8'))
"""