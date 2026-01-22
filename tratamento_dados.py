#!/usr/bin/env python3
import csv
from pathlib import Path

try:
    import matplotlib.pyplot as plt
except ImportError:  # pragma: no cover - runtime dependency
    plt = None

def parse_filename(filename: str):
    name = filename
    if name.startswith("saida_"):
        name = name[len("saida_"):]
    if name.endswith(".txt"):
        name = name[:-len(".txt")]
    if "_" in name:
        alg, size = name.rsplit("_", 1)
    else:
        alg, size = name, ""
    return alg, size


def read_stats(path: Path):
    with path.open("r", encoding="utf-8") as f:
        lines = [line.strip() for line in f if line.strip()]
    if len(lines) < 3:
        raise ValueError(f"Arquivo sem linhas suficientes: {path}")
    stats = lines[-3:]
    values = {}
    for item in stats:
        if ":" not in item:
            continue
        key, value = item.split(":", 1)
        values[key.strip()] = value.strip()
    return (
        values.get("comparacoes", ""),
        values.get("movimentacoes", ""),
        values.get("tempo_ms", ""),
    )


def collect_rows(base_dir: Path):
    mapping = {
        "Resultados_aleatorio": "aleatorio",
        "Resultados_decrescente": "decrescente",
        "Resultados_crescente": "crescente",
    }
    rows = []
    for folder, entrada in mapping.items():
        folder_path = base_dir / folder
        if not folder_path.is_dir():
            continue
        for file in folder_path.iterdir():
            if not file.is_file() or not file.name.endswith(".txt"):
                continue
            algoritmo, tamanho = parse_filename(file.name)
            comparacoes, movimentacoes, tempo = read_stats(file)
            entrada_label = f"{entrada}_{tamanho}" if tamanho else entrada
            rows.append((
                algoritmo,
                entrada_label,
                comparacoes,
                movimentacoes,
                tempo,
                int(tamanho) if tamanho.isdigit() else 0,
            ))
    rows.sort(key=lambda r: (r[0], r[1], r[5]))
    return rows


def parse_entrada_label(label: str):
    if "_" in label:
        tipo, size = label.rsplit("_", 1)
    else:
        tipo, size = label, ""
    return tipo, size


def build_data(rows):
    data = []
    for algoritmo, entrada, comparacoes, movimentacoes, tempo, _ in rows:
        tipo, tamanho_str = parse_entrada_label(entrada)
        tamanho = int(tamanho_str) if tamanho_str.isdigit() else None
        try:
            comparacoes_val = float(comparacoes)
        except ValueError:
            comparacoes_val = None
        try:
            movimentacoes_val = float(movimentacoes)
        except ValueError:
            movimentacoes_val = None
        try:
            tempo_val = float(tempo)
        except ValueError:
            tempo_val = None
        data.append({
            "algoritmo": algoritmo,
            "entrada_tipo": tipo,
            "tamanho": tamanho,
            "comparacoes": comparacoes_val,
            "movimentacoes": movimentacoes_val,
            "tempo_ms": tempo_val,
        })
    return data


def plot_series(x_vals, y_vals, label):
    if not x_vals:
        return
    plt.plot(x_vals, y_vals, marker="o", linewidth=2, label=label)


def generate_graphs(data, output_dir: Path):
    if plt is None:
        print("matplotlib nao encontrado. Instale com: pip3 install matplotlib")
        return
    output_dir.mkdir(exist_ok=True)
    algoritmos = sorted({d["algoritmo"] for d in data})
    entrada_tipos = sorted({d["entrada_tipo"] for d in data})
    metrics = [
        ("comparacoes", "Comparacoes"),
        ("movimentacoes", "Movimentacoes"),
        ("tempo_ms", "Tempo(ms)"),
    ]

    for alg in algoritmos:
        for metric_key, metric_label in metrics:
            plt.figure(figsize=(8, 5))
            for tipo in entrada_tipos:
                points = [
                    (d["tamanho"], d[metric_key])
                    for d in data
                    if d["algoritmo"] == alg and d["entrada_tipo"] == tipo
                    and d["tamanho"] is not None and d[metric_key] is not None
                ]
                points.sort(key=lambda p: p[0])
                x_vals = [p[0] for p in points]
                y_vals = [p[1] for p in points]
                plot_series(x_vals, y_vals, tipo)
            plt.title(f"{alg} - {metric_label} por entrada")
            plt.xlabel("Tamanho")
            plt.ylabel(metric_label)
            plt.grid(True, linestyle="--", alpha=0.3)
            plt.legend()
            out_path = output_dir / f"{metric_key}_por_algoritmo_{alg}.png"
            plt.tight_layout()
            plt.savefig(out_path, dpi=150)
            plt.close()

    for tipo in entrada_tipos:
        for metric_key, metric_label in metrics:
            plt.figure(figsize=(8, 5))
            for alg in algoritmos:
                points = [
                    (d["tamanho"], d[metric_key])
                    for d in data
                    if d["algoritmo"] == alg and d["entrada_tipo"] == tipo
                    and d["tamanho"] is not None and d[metric_key] is not None
                ]
                points.sort(key=lambda p: p[0])
                x_vals = [p[0] for p in points]
                y_vals = [p[1] for p in points]
                plot_series(x_vals, y_vals, alg)
            plt.title(f"{metric_label} - entrada {tipo}")
            plt.xlabel("Tamanho")
            plt.ylabel(metric_label)
            plt.grid(True, linestyle="--", alpha=0.3)
            plt.legend()
            out_path = output_dir / f"{metric_key}_por_entrada_{tipo}.png"
            plt.tight_layout()
            plt.savefig(out_path, dpi=150)
            plt.close()


def main():
    base_dir = Path(__file__).resolve().parent
    output_path = base_dir / "estatisticas.csv"
    rows = collect_rows(base_dir)
    with output_path.open("w", newline="", encoding="utf-8") as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(["Algoritmo", "Entrada", "Comparacoes", "Movimentacoes", "Tempo(ms)"])
        for algoritmo, entrada, comparacoes, movimentacoes, tempo, _ in rows:
            writer.writerow([algoritmo, entrada, comparacoes, movimentacoes, tempo])
    print(f"CSV gerado em: {output_path}")
    data = build_data(rows)
    generate_graphs(data, base_dir / "graficos")


if __name__ == "__main__":
    main()
