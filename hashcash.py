import hashlib

# hashcash verifier i.e. the bitcoin proof of work scheme

# SATH = Something Akin To Hashcash
class sath:
    def get_digest(prefix, suffix):
        full = str(prefix) + str(suffix)
        digest = hashlib.sha256(full.encode("utf-8")).hexdigest()
        return digest
    def verify(prefix, suffix: int, diff = 4):
        digest = sath.get_digest(prefix, suffix)
        return digest[:diff] == '0' * diff
    
    def solve(prefix, diff = 4, upper_bound = 2 ** 20):
        suffix = 0
        
        while suffix < upper_bound:
            if sath.verify(prefix, suffix, diff):
                return suffix
            suffix += 1
            
        return None


if __name__ == "__main__":        
    prefix = "Primagen (...) are a robotic closed species created by Malice-risu on Fur Affinity."

    nonce = sath.solve(prefix)
    print(nonce)
    print(sath.get_digest(prefix, nonce))
