#!/usr/bin/env python

import shutil, re, os, sys

file_model = "Model.template"
bookname = "TechNotes"
file_bibtex = "thebib.bib"
folder_target = "./pdf/"

#if name is a chapter, return its sections
def get_sections(name):
    if not os.path.isdir(name):
        return [] 
    files = os.listdir(name)
    sections = []
    for section in files:
        if re.match('.*\.tex$', section) and not re.match(".*lmz0610.*", section):
            sections.append(name + "/" + section)
    return sections

def is_updated(pdffile, texfiles):

    def depend_modified(fname, ims):
        depend_mtime = os.path.getmtime(fname)
        if depend_mtime > ims:
            print (pdffile + ' mtime: ' + str(ims)) 
            print (fname + ' mtime: ' + str(depend_mtime))
            return True 
        return False


    old_pdffile = folder_target + pdffile
    if not os.path.isfile(old_pdffile): 
        return False

    pdf_mtime = os.path.getmtime(old_pdffile)
    #if depend_modified(sys.argv[0], pdf_mtime):
        #return False
    #if depend_modified(file_model, pdf_mtime):
        #return False

    for section in texfiles:
        if depend_modified(section, pdf_mtime):
            return False

    return True 

def remove_tmp(tmpname):
    if os.path.isfile(tmpname): 
        os.remove(tmpname)

def remove_latex_tmps(texname):
    remove_tmp(texname + ".pdf")
    remove_tmp(texname + ".tex")
    remove_tmp(texname + ".blg")
    remove_tmp(texname + ".bbl")
    remove_tmp(texname + ".out")
    remove_tmp(texname + ".toc")
    remove_tmp(texname + ".aux")
    remove_tmp(texname + ".idx")
    remove_tmp(texname + ".log")
    remove_tmp(texname + ".lof")
    remove_tmp(texname + ".lot")


def read_bbl_file(object_name):
    file_bbl = object_name + ".bbl"
    if not os.path.isfile(file_bbl):
        return ""
    with open(file_bbl, 'r') as f:
        return f.read()

#if depend_files contains citation
def need_bibtex(object_name, depend_files):
    #if a file contains latex citation command \cite{}
    def contain_citation(section_name):
        with open(section_name, "r") as f:
            content_section = f.read()
            if content_section.find("\\cite{") == -1:
                return False
        return True
    
    for section in depend_files:
        if contain_citation(section):
            return True

    return False 


def gen_pdf(object_name):
    object_pdf = object_name + ".pdf"

    if object_name == bookname:
        depend_files = book_sections
        targets = [folder_target + object_pdf, folder_target + "AAAAAAAAAAA.pdf"]
        chapter_start_counter = 0
    else:
        depend_files = chap_sections[object_name]
        targets = [folder_target + object_pdf]
        chapter_start_counter = book_chapters.index(object_name)


    if is_updated(object_pdf, depend_files):
        print(object_pdf + " is updated")
        #return False

    obj_need_bibtex = need_bibtex(object_name, depend_files)

    model = ''
    with open(file_model) as model_file:
        model = model_file.read()

    model = model.replace("OBJECTNAME", object_name)

    if object_name == 'Report':
        model = model.replace("CHAPTERSTART", "0")
        model = model.replace("\\tableofcontents", "%\\tableofcontents")
        model = model.replace("ctexrep", "ctexart")
        model = model.replace("\\setcounter{chapter}", "%\\setcounter{chapter}")
    else:
        model = model.replace("CHAPTERSTART", str(chapter_start_counter))

    insert_word = "TOADD"
    insert_pos = model.find(insert_word)

    latex_text = model[:insert_pos] + insert_word
    
    for section in depend_files: 
        latex_text = latex_text + "\n\\input{"+ section + "}"
    
    if obj_need_bibtex:
        latex_text = latex_text + "\n\n"
        latex_text = latex_text + "\\bibliographystyle{unsrt}\n"
        latex_text = latex_text + "\\bibliography{thebib}\n"

    latex_text = latex_text + model[insert_pos+len(insert_word):]

    object_tex = object_name + ".tex"
    with open(object_tex, "w") as f:
        f.write(latex_text)

    os.system("xelatex " + object_name)
    if len(sys.argv) < 3 or sys.argv[2] != "fast":
        if obj_need_bibtex:
            old_bbl = read_bbl_file(object_name)
            os.system("bibtex " + object_name)
            if old_bbl != read_bbl_file(object_name):
                os.system("xelatex " + object_name)
        os.system("xelatex " + object_name)

    if os.path.isfile(object_pdf): 
        for target in targets:
            shutil.copy(object_pdf, target)

    return True

#trim trailing slash
def trim_chap_name(name):
    if name[len(name) - 1] == '/':
        name = name[:len(name)-1]
    return name

def merge_chapter_pdfs():
    mergecmd = 'pdftk '
    for chap in book_chapters:
        chappdf = folder_target + chap + '.pdf'
        if os.path.isfile(chappdf):
            mergecmd += chappdf + ' ' 
    mergecmd += 'cat output ' + folder_target + 'AAABBBBBBBB.pdf'
    print (mergecmd)
    os.system(mergecmd)

##################################################

#now work starts


files = os.listdir('.')
chap_sections = {}
book_sections = []
book_chapters = []
for chap in files:
    sections = get_sections(chap)
    if len(sections):
        chap_sections[chap] = sections
        book_sections.extend(sections)
        book_chapters.append(chap)


if len(sys.argv) < 2:
    print("Usage:\n") 
    print("\t" + sys.argv[0] + " CHAPTER ")
    print("\t" + sys.argv[0] + " one ")
    print("\t" + sys.argv[0] + " all ")
    sys.exit(1)

cmd = sys.argv[1]
if cmd == "one":
    gen_pdf(bookname)
elif cmd == "all":
    modified = False
    for chap in chap_sections:
        modified = gen_pdf(chap) or modified
    if modified:
        merge_chapter_pdfs()
elif cmd == "clean":
    for chap in chap_sections:
        remove_latex_tmps(chap)
    remove_latex_tmps(bookname)
else:
    chap = trim_chap_name(cmd)
    if chap in book_sections:
        #chap is actually a section
        section = chap
        chap = 'Report'
        chap_sections[chap] = [section]
        book_chapters.append(chap)

    if not chap_sections.has_key(chap):
        print(chap + " is not a valid chapter name")
        sys.exit(1)
    modified = gen_pdf(chap)
    if modified and chap != 'Report':
        merge_chapter_pdfs()




