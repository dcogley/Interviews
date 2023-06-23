const STR: &str = "This is a coding test, I like coding because it is fun";

fn main() {
    println!("Input: {}", STR);

    let stream: Vec<&str> = STR.split(" ").collect();

    // val++
    let postinc = |i: &mut usize| -> usize {
        *i += 1;
        return *i - 1;
    };

    // check for duplicates and append to words
    let mut idx: usize = 0;
    let mut words: Vec<&str> = Vec::with_capacity(stream.capacity());
    
    for word in stream {
        if !words.contains(&word) {
            words.insert(postinc(&mut idx), word);
        }
    }

    // reverse the vector
    words.reverse();

    println!("Output: {}", words.join(" "));
}
