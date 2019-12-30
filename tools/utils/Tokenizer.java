package utils;

public class Tokenizer {
    private int m_start = 0;
    private String m_str;

    public Tokenizer(String str) {
        m_str = str;
    }

    public String nextToken() {
        int i = m_start;
        for(; i < m_str.length(); ++i) {
            if(!Character.isWhitespace(m_str.charAt(i))) {
                break;
            }
        }

        if(i >= m_str.length()) {
            return null;
        }

        int j = i + 1;
        if(m_str.charAt(i) != '"') {
            for(; j < m_str.length(); ++j) {
                if(Character.isWhitespace(m_str.charAt(j))) {
                    break;
                }
            }
        } else {
            for(; j < m_str.length(); ++j) {
                if(m_str.charAt(j) == '"') {
                    break;
                }
            }
        }

        m_start = j + 1;
        return m_str.charAt(i) != '"' ? m_str.substring(i, j) : m_str.substring(i+1, j);
    }
}