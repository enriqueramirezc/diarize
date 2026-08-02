import pandas as pd
import matplotlib.pyplot as plt

df = pd.read_csv("output/P27.csv")

plt.fill_between(df["Time"], df["Min"], df["Max"], color="steelblue", alpha=0.6)
plt.xlabel("Time (s)")
plt.ylabel("Amplitude")
plt.title("Waveform Envelope")
plt.tight_layout()
plt.show()