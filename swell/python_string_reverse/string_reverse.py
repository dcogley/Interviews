
class StringReversal:
    def __init__(self):
        self.str = "This is a coding test, I like coding because it is fun"
        print("Input: " + self.str)

    def reverse_string(self):
        words = self.str.split(" ")
        new_list = []

        # remove any duplicates
        for word in words:
            if word not in new_list:
                new_list.append(word)
            
        new_list.reverse()
        self.str = ' '.join(new_list)
        print("Output: " + self.str)

stringreverse = StringReversal()
stringreverse.reverse_string()
