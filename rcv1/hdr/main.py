import os
import pdb 


for filename in os.listdir("."):
    lines = []
    if filename.endswith(".h") or filename.endswith(".c"):
        with open( filename ) as file:
            for line in file:
                comment = line.find("/*") 
                limit = len(line)
                if comment != -1: 
                    limit = comment
                r = line[0:limit].rfind("/")    
                h = line[0:limit].rfind(".h")
                c = line[0:limit].rfind("\"") 
                if line.__contains__("#include" ) and r != -1 and c != -1: 
                    print("#include \"" + line[r+1:len(line)])
                    lines.append("#include \"" + line[r+1:len(line)])
                else:
                    print(line)
                    lines.append(line)

        os.remove(filename)
        outfile = open(filename, 'w')
        outfile.writelines([line for line in lines])
        outfile.close()
    else:
        continue

