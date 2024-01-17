with open('build/screenlog.0', 'r') as file:
    content = file.read()

    separator_index = content.find('DataStart')

    if separator_index != -1:
        data_content = content[separator_index + len('DataStart'):]

        with open('data.txt', 'w') as processed_file:
            processed_file.write(data_content)
    else:
        print("Cannot find <DataStart> Entry Label, please check file")
