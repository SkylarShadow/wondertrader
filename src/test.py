import os

OLD = "ZT"
NEW = "ZT"

root = os.getcwd()

print("==== Renaming files ====")

for dirpath, dirnames, filenames in os.walk(root):
    for name in filenames:
        if OLD in name:  # 大小写敏感
            newname = name.replace(OLD, NEW)
            oldpath = os.path.join(dirpath, name)
            newpath = os.path.join(dirpath, newname)

            print(f"FILE: {oldpath} -> {newname}")
            os.rename(oldpath, newpath)

print("\n==== Renaming directories ====")

# 收集目录
dirs = []
for dirpath, dirnames, filenames in os.walk(root):
    for d in dirnames:
        if OLD in d:
            dirs.append(os.path.join(dirpath, d))

# 倒序（避免父目录先改名）
for d in sorted(dirs, key=len, reverse=True):
    dirname = os.path.basename(d)
    newdirname = dirname.replace(OLD, NEW)

    newpath = os.path.join(os.path.dirname(d), newdirname)

    print(f"DIR: {d} -> {newdirname}")
    os.rename(d, newpath)

print("\n==== Done ====")