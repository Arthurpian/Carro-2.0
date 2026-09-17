"""
Carroceria estilo "buggy de corrida" (design ORIGINAL, sem reproduzir o
personagem licenciado Relampago McQueen) para encaixar sobre o chassi
LAFVIN 4WD do Arthur.

Todas as medidas vieram das fotos + medidas reais passadas pelo Arthur:
  - largura roda-a-roda (fora a fora): 220 mm
  - roda: 65 mm de diametro, com 10 mm de vao entre a roda e a placa central
  - placa central exposta entre as rodas: 70 mm  (65+10+70+10+65 = 220 OK)
  - comprimento do chassi (frente-tras): 152 mm
  - altura total ate o topo da antena: 157 mm

Duas medidas NAO temos ainda (foram estimadas -- ver ASSUNCOES abaixo):
  - altura do chao ate o tampo/deck do chassi
  - altura da pilha de eletronica (bateria + driver + ESP32-CAM), sem a antena
Ajuste GROUND_TO_DECK e STACK_HEIGHT se a peca ficar baixa/alta demais.
"""
import numpy as np
import trimesh
from scipy.interpolate import PchipInterpolator
from trimesh.creation import extrude_polygon
from shapely.geometry import Polygon

rng = np.random.default_rng(0)

# ----------------------------------------------------------------------
# 1) DIMENSOES REAIS DO CHASSI (medidas do Arthur)
# ----------------------------------------------------------------------
CHASSIS_LENGTH            = 152.0   # frente -> tras
CHASSIS_WIDTH_OVER_WHEELS = 220.0   # roda esquerda (fora) -> roda direita (fora)
WHEEL_DIAMETER            = 65.0
WHEEL_GAP                 = 10.0    # vao roda <-> placa central, de cada lado
FRONT_AXLE_INSET          = 22.0    # ASSUNCAO: distancia do eixo a borda do chassi

# ASSUNCOES (meca e ajuste se necessario!)
GROUND_TO_DECK = 40.0   # altura do chao ate o tampo do chassi
STACK_HEIGHT   = 60.0   # altura livre necessaria acima do chassi p/ bateria+driver+ESP32-CAM (sem antena)

# ----------------------------------------------------------------------
# 2) DIMENSOES DA CARROCERIA (maior que o chassi, estilo buggy)
# ----------------------------------------------------------------------
NOSE_OVERHANG    = 42.0
TAIL_OVERHANG    = 48.0
FENDER_CLEARANCE = 6.0   # folga lateral extra sobre a largura roda-a-roda

BODY_LENGTH = NOSE_OVERHANG + CHASSIS_LENGTH + TAIL_OVERHANG           # 242 mm
BODY_WIDTH  = CHASSIS_WIDTH_OVER_WHEELS + 2 * FENDER_CLEARANCE          # 232 mm
DECK        = GROUND_TO_DECK
ROOFZ       = GROUND_TO_DECK + STACK_HEIGHT                              # 100 mm

WALL_T    = 1.5   # = 3 perimetros x linha de 0.5mm (bate certinho, sem sobra p/ infill)
FIT_CLEAR = 0.15
PLUG_LEN  = 6.0

NOSE_SOLID = 16.0
TAIL_SOLID = 18.0

ANTENNA_HOLE_D     = 14.0
ANTENNA_HOLE_XFRAC = 0.16   # perto da frente, onde ficam camera/antena nas fotos
ANTENNA_HOLE_Y     = 0.0

# posicao das rodas (coord. X da carroceria, 0 = nariz)
WHEEL_X_FRONT = NOSE_OVERHANG + FRONT_AXLE_INSET
WHEEL_X_REAR  = NOSE_OVERHANG + CHASSIS_LENGTH - FRONT_AXLE_INSET
ARCH_RADIUS   = WHEEL_DIAMETER / 2 + 8.0
ARCH_Z        = WHEEL_DIAMETER / 2

# corte em 2 partes (a peca toda cabe na mesa, mas cortamos assim mesmo
# para deitar cada metade e imprimir sem suporte)
CUT_X = 115.0

print("BODY_LENGTH", BODY_LENGTH, "BODY_WIDTH", BODY_WIDTH, "ROOFZ", ROOFZ)
print("front arch span", WHEEL_X_FRONT - ARCH_RADIUS, WHEEL_X_FRONT + ARCH_RADIUS)
print("rear  arch span", WHEEL_X_REAR - ARCH_RADIUS, WHEEL_X_REAR + ARCH_RADIUS)

# ----------------------------------------------------------------------
# 3) PERFIL (estacoes) -- silhueta tipo buggy, 100% original
#    (x_frac, half_width, bottom_z, top_z)
# ----------------------------------------------------------------------
stations = np.array([
    [0.00,  10,  DECK + 4,  DECK + 16],
    [0.07,  70,  DECK - 2,  DECK + 28],
    [0.16, 112,  DECK - 6,  DECK + 38],   # capo / para-lama frente (sobre rodas dianteiras)
    [0.30,  90,  DECK + 0,  DECK + 62],
    [0.40,  70,  DECK + 0,  ROOFZ - 2],
    [0.50,  62,  DECK + 0,  ROOFZ + 0],   # teto
    [0.60,  70,  DECK + 0,  ROOFZ - 4],
    [0.70,  88,  DECK + 0,  DECK + 50],
    [0.84, 112,  DECK - 6,  DECK + 38],   # para-lama traseiro (sobre rodas traseiras)
    [0.94,  75,  DECK + 0,  DECK + 22],
    [1.00,  10,  DECK + 4,  DECK + 14],
])
xs = stations[:, 0] * BODY_LENGTH
hw_i = PchipInterpolator(xs, stations[:, 1])
bz_i = PchipInterpolator(xs, stations[:, 2])
tz_i = PchipInterpolator(xs, stations[:, 3])

N = 32          # pontos ao redor da secao transversal
N_TOP = 2.3
N_BOT = 3.6
N_SIDE = 2.6
BELT_FRAC = 0.18  # onde fica a "linha de cintura" (largura maxima) dentro da altura


def cross_section(x, shrink=0.0, bottom_extend=None):
    """Retorna Nx2 (y,z) de uma secao transversal tipo 'super-elipse assimetrica'."""
    hw = max(hw_i(x) - shrink, 1.0)
    bottom = bz_i(x) + (shrink if shrink > 0 else 0.0)
    top = tz_i(x) - shrink
    if bottom_extend is not None:
        bottom = bottom_extend
    z_mid = bottom + (top - bottom) * BELT_FRAC
    up_h = top - z_mid
    lo_h = z_mid - bottom
    th = np.linspace(0, 2 * np.pi, N, endpoint=False)
    c, s = np.cos(th), np.sin(th)
    y = hw * np.sign(c) * np.abs(c) ** (2.0 / N_SIDE)
    z = np.where(
        s >= 0,
        z_mid + up_h * np.abs(s) ** (2.0 / N_TOP),
        z_mid - lo_h * np.abs(s) ** (2.0 / N_BOT),
    )
    return np.column_stack([y, z])


def build_loft(x0, x1, m_stations, shrink=0.0, bottom_extend=None, cap_start=True, cap_end=True):
    xs_local = np.linspace(x0, x1, m_stations)
    verts = []
    for x in xs_local:
        pts = cross_section(x, shrink=shrink, bottom_extend=bottom_extend)
        verts.append(np.column_stack([np.full(N, x), pts]))
    verts = np.array(verts)  # (M, N, 3)
    M = m_stations
    faces = []
    vflat = verts.reshape(-1, 3)

    def idx(i, j):
        return i * N + (j % N)

    for i in range(M - 1):
        for j in range(N):
            a = idx(i, j)
            b = idx(i, j + 1)
            c = idx(i + 1, j)
            d = idx(i + 1, j + 1)
            faces.append([a, b, d])
            faces.append([a, d, c])
    vlist = vflat.tolist()
    faces = list(faces)

    if cap_start:
        centroid = verts[0].mean(axis=0)
        c_idx = len(vlist)
        vlist.append(centroid.tolist())
        for j in range(N):
            a = idx(0, j)
            b = idx(0, j + 1)
            faces.append([c_idx, b, a])
    if cap_end:
        centroid = verts[-1].mean(axis=0)
        c_idx = len(vlist)
        vlist.append(centroid.tolist())
        last = M - 1
        for j in range(N):
            a = idx(last, j)
            b = idx(last, j + 1)
            faces.append([c_idx, a, b])

    mesh = trimesh.Trimesh(vertices=np.array(vlist), faces=np.array(faces), process=True)
    return mesh


print("Construindo casco externo...")
outer = build_loft(0.0, BODY_LENGTH, 121)
print("outer watertight?", outer.is_watertight, "volume", outer.volume)

print("Construindo cavidade interna (oca, fundo aberto)...")
cav_x0 = NOSE_SOLID
cav_x1 = BODY_LENGTH - TAIL_SOLID
cavity = build_loft(cav_x0, cav_x1, 101, shrink=WALL_T, bottom_extend=-500.0)
print("cavity watertight?", cavity.is_watertight)

print("Subtraindo cavidade (casca oca com fundo aberto)...")
hollow = trimesh.boolean.difference([outer, cavity], engine="manifold")
print("hollow watertight?", hollow.is_watertight, "bodies:", hollow.body_count)

# ----------------------------------------------------------------------
# 4) Recortes dos para-lamas (arcos de roda) -- 4 cilindros no eixo Y
# ----------------------------------------------------------------------
def wheel_arch(x_center):
    cyl = trimesh.creation.cylinder(radius=ARCH_RADIUS, height=BODY_WIDTH + 40, sections=48)
    # cylinder criado ao longo de Z por padrao -> rotacionar para o eixo Y
    R = trimesh.transformations.rotation_matrix(np.pi / 2, [1, 0, 0])
    cyl.apply_transform(R)
    cyl.apply_translation([x_center, 0, ARCH_Z])
    return cyl

arches = trimesh.util.concatenate([wheel_arch(WHEEL_X_FRONT), wheel_arch(WHEEL_X_REAR)])
print("Recortando arcos de roda...")
body = trimesh.boolean.difference([hollow, arches], engine="manifold")
print("body watertight?", body.is_watertight, "bodies:", body.body_count)

# ----------------------------------------------------------------------
# 5) Furo para a antena (perto da frente, no teto)
# ----------------------------------------------------------------------
ant_x = ANTENNA_HOLE_XFRAC * BODY_LENGTH
ant_top = tz_i(ant_x) + 5
ant_cyl = trimesh.creation.cylinder(radius=ANTENNA_HOLE_D / 2, height=40, sections=32)
ant_cyl.apply_translation([ant_x, ANTENNA_HOLE_Y, ant_top - 15])
print("Furando passagem da antena em x=%.1f..." % ant_x)
body = trimesh.boolean.difference([body, ant_cyl], engine="manifold")
print("body (c/ furo antena) watertight?", body.is_watertight, "bodies:", body.body_count)

body.export("/tmp/claude-0/-home-claude/b0138ecc-d494-5e18-928d-daf5f0dd46d1/scratchpad/body_full.stl")
print("Exportado body_full.stl. Bounds:", body.bounds)
